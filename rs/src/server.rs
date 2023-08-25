use std::{pin::Pin, time::Duration};
use prost::Message;
use tokio_stream::Stream;
use tonic::{transport::Server, Request, Response, Status};
use tonic_reflection::pb::FILE_DESCRIPTOR_SET;
use std::collections::HashMap;
use std::sync::Arc;
use tokio::sync::{mpsc, mpsc::{Sender, Receiver}, Mutex};
use tokio_stream::wrappers::ReceiverStream;
use prost_types::FileDescriptorSet;
use prost_reflect::{DynamicMessage, DescriptorPool, ReflectMessage};

use farm_ng::core::{
    event_service_server::{EventService, EventServiceServer},
    ListUrisReply, ListUrisRequest,
    RequestReplyReply, RequestReplyRequest,
    SubscribeReply, SubscribeRequest,
    Uri, Timestamp
};

pub mod farm_ng {
    pub mod core {
        tonic::include_proto!("farm_ng.core.proto");
    }
}

type SubscribeResult<T> = Result<Response<T>, Status>;
type SubscribeResponseStream = Pin<Box<dyn Stream<Item = Result<SubscribeReply, Status>> + Send>>;

#[derive(Debug)]
struct SubscribeReplyQueue {
    tx: Arc<Mutex<Sender<SubscribeReply>>>,
    rx: Arc<Mutex<Receiver<SubscribeReply>>>,
}

impl SubscribeReplyQueue {
    fn new(capacity: usize) -> Self {
        let (tx, rx) = tokio::sync::mpsc::channel(capacity);
        Self { tx: Arc::new(Mutex::new(tx)), rx: Arc::new(Mutex::new(rx)) }
    }
}

// defining a struct for our service
#[derive(Debug)]
pub struct EventServiceGrpc {
    client_queues: Arc<Mutex<HashMap<String, Vec<SubscribeReplyQueue>>>>,
}

// implementing rpc for service defined in .proto
#[tonic::async_trait]
impl EventService for EventServiceGrpc {

    type subscribeStream = SubscribeResponseStream;

    // TODO: implement me
    async fn list_uris(
        &self,
        request: Request<ListUrisRequest>
    )->Result<Response<ListUrisReply>, Status>{
        println!("Got a request: {:?}", request);

        let reply = ListUrisReply {
            uris: vec![Uri {
                scheme: String::new(),
                authority: String::new(),
                path: String::new(),
                query: String::from("hello"),
            }],
        };

        Ok(Response::new(reply))
    }

    // TODO: implement me
    async fn request_reply(
        &self,
        request: Request<RequestReplyRequest>
    )->Result<Response<RequestReplyReply>, Status>{
        println!("Got a request: {:?}", request);

        let reply = RequestReplyReply {
            event: None,
            payload: Vec::<u8>::new(),
        };

        Ok(Response::new(reply))
    }

    // TODO: for some reason it get disconnected after every call
    async fn subscribe(
        &self,
        request: Request<SubscribeRequest>,
    )->SubscribeResult<Self::subscribeStream> {
        println!("Got a request: {:?}", request);

        let uri_path = request.get_ref().clone().uri.unwrap().path;
        println!("URI path: {}", uri_path);

        // safely create first a queue for the client
        let queue = SubscribeReplyQueue::new(10);
        let rx = queue.rx.clone();

        {
            let mut client_queues = self.client_queues.lock().await;

            if  !client_queues.contains_key(&uri_path) {
                client_queues.insert(uri_path.clone(), vec![]);
            } else {
                println!("Client queue already exists for path: {}", uri_path);
            }
            // add the client queue to the list of queues for this path
            client_queues.get_mut(&uri_path).unwrap().push(queue);
            println!("Added client queue for path: {}", uri_path);
        }

        // pass the client queue to the client
        let client_queues = self.client_queues.clone();

        let (tx, rx_out) = mpsc::channel(4);
        tokio::spawn(async move {
            while let Some(item) = rx.lock().await.recv().await {
                match tx.send(Ok(item)).await {
                    Ok(_) => {
                        // item (server response) was queued to be send to client
                        // print!("Sent to queue: {}", path);
                    }
                    Err(_item) => {
                        // output_stream was build from rx and both are dropped
                        // println!("Error sending to queue");
                        break;
                    }
                }
            }
            println!("\tclient disconnected");

            // remove the client queue from the list of queues for this path
            client_queues.lock().await.remove(uri_path.as_str());
        });

        let output_stream = ReceiverStream::new(rx_out);
        Ok(Response::new(
            Box::pin(output_stream) as Self::subscribeStream
        ))

    }

}

impl EventServiceGrpc {
    fn new() -> Self {
        Self {
            client_queues: Arc::new(Mutex::new(HashMap::new())),
        }
    }

    //async fn publish<T: protobuf::Message + std::fmt::Debug>(
    async fn publish<T: prost::Message + std::fmt::Debug>(
        &self,
        path: String,
        message: T,
        timestamps: Option<Vec<Timestamp>>,
        latch: bool,
    ) {

        //println!("Publish called with path: {}", path);
        //println!("Message: {:?}", message);

        //let payload = message.write_to_bytes().unwrap();
        let payload = message.encode_to_vec();
        //println!("Payload: {:?}", payload);

        let file_descriptor_set = prost_types::FileDescriptorSet::decode(FILE_DESCRIPTOR_SET).unwrap();

        // TODO: get correct data from the descriptor
        let service_name = "event_service";
        let message_type = "farm_ng.core.SubscribeReply";
        let pb_type = "farm_ng.core.proto";

        let uri = farm_ng::core::Uri {
            scheme: "protobuf".to_string(),
            authority: "hostname".to_string(),
            path: path.clone(),
            query: format!(
                "type={}&pb={}&service_name={}", message_type, pb_type, service_name
            ).to_string()
        };
        //println!("URI: {:?}", uri);

        // TODO: get correct timestamps and counts

        let event = farm_ng::core::Event {
            uri: Some(uri),
            timestamps: timestamps.unwrap_or(vec![]),
            payload_length: payload.len() as i64,
            sequence: 0,
        };
        //println!("Event: {:?}", event);

        //println!("################");

        let reply = farm_ng::core::SubscribeReply {
            event: Some(event),
            payload: payload,
        };

        // get all queues for this path
        let client_queues = self.client_queues.lock().await;
        let queues = match client_queues.get(&path.clone()) {
            Some(queues) => queues,
            None => {
                println!("Number of clients: {}", 0);
                return;
            }
        };

        let num_clients = queues.len();
        println!("Number of clients: {}", num_clients);

        for queue in queues {
            let tx = queue.tx.lock().await;
            match tx.send(reply.clone()).await {
                Ok(_) => {
                    // item (server response) was queued to be send to client
                    // print!("Sent to queue: {}", path);
                }
                Err(_item) => {
                    // output_stream was build from rx and both are dropped
                    println!("Error sending to queue");
                }
            }
        }

    }

}

async fn test_send_smoke(task_id: u8, delay_millis: u64, event_service: Arc<EventServiceGrpc>) {
    let mut counter = 0;
    loop {
        // convert counter to Message
        let counter_msg = String::from(format!("{}: {}", task_id, counter));

        event_service.publish(
            String::from("foo"),
            counter_msg,
            None,
            false,
        ).await;

        tokio::time::sleep(Duration::from_millis(delay_millis)).await;
        //println!("task_id: {}, counter: {}", task_id, counter);
        counter += 1;
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr= "[::1]:50051".parse()?;

    // creating a service
    //let event_service = EventServiceGrpc::default();
    let event_service = EventServiceGrpc::new();
    println!("Server listening on {}", addr);

    //let event_service_arc = Arc::new(Mutex::new(event_service));
    let event_service_arc = Arc::new(event_service);

    // adding our service to our server.
    let server = Server::builder()
        .add_service(EventServiceServer::from_arc(event_service_arc.clone()));

    // running our server

    let _ = tokio::join!(
        server.serve(addr),
        test_send_smoke(0, 100, event_service_arc.clone()),
        test_send_smoke(1, 200, event_service_arc.clone()),
        test_send_smoke(2, 50, event_service_arc.clone()),
    );

    Ok(())

}
