use std::{pin::Pin, time::Duration};
use std::sync::Arc;
use tokio::sync::mpsc;
use tokio_stream::{wrappers::ReceiverStream, Stream, StreamExt};
use tonic::{transport::Server, Request, Response, Status};
use protobuf::well_known_types::wrappers::Int32Value;
use protobuf::Message;
use protobuf::descriptor::FileDescriptorProto;

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

// defining a struct for our service
#[derive(Debug, Default)]
pub struct EventServiceGrpc {}

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

    async fn subscribe(
        &self,
        request: Request<SubscribeRequest>,
    )->SubscribeResult<Self::subscribeStream> {
        println!("Got a request: {:?}", request);

        let repeat = std::iter::repeat(SubscribeReply {
            event: None,
            payload: Vec::<u8>::new(),
        });
        let mut stream = Box::pin(tokio_stream::iter(repeat).throttle(Duration::from_millis(200)));

        // spawn and channel are required if you want handle "disconnect" functionality
        // the `out_stream` will not be polled after client disconnect
        let (tx, rx) = mpsc::channel(128);
        tokio::spawn(async move {
            while let Some(item) = stream.next().await {
                match tx.send(Result::<_, Status>::Ok(item)).await {
                    Ok(_) => {
                        // item (server response) was queued to be send to client
                    }
                    Err(_item) => {
                        // output_stream was build from rx and both are dropped
                        break;
                    }
                }
            }
            println!("\tclient disconnected");
        });

        let output_stream = ReceiverStream::new(rx);
        Ok(Response::new(
            Box::pin(output_stream) as Self::subscribeStream
        ))

    }

}

impl EventServiceGrpc {
    async fn publish<T: protobuf::Message + std::fmt::Debug>(
        &self,
        path: String,
        message: T,
        timestamps: Option<Vec<Timestamp>>,
        latch: bool,
    ) {

        println!("Publish called with path: {}", path);
        println!("Message: {:?}", message);

        let payload = message.write_to_bytes().unwrap();
        println!("Payload: {:?}", payload);

        // TODO: implement me
        let msg_descriptor = FileDescriptorProto::parse_from_bytes(&payload).unwrap();
        println!("Message descriptor: {:?}", msg_descriptor);

        // TODO: get correct hostname and service name
        let service_name = "event_service";

        let uri = farm_ng::core::Uri {
            scheme: String::from("protobuf"),
            authority: String::from("invalid_hostname"),
            path: path,
            query: String::from(format!("service_name={}", service_name))
        };
        println!("URI: {:?}", uri);

        // TODO: get correct timestamps and counts

        let event = farm_ng::core::Event {
            uri: Some(uri),
            timestamps: timestamps.unwrap_or(vec![]),
            payload_length: payload.len() as i64,
            sequence: 0,
        };
        println!("Event: {:?}", event);

        let reply = farm_ng::core::SubscribeReply {
            event: Some(event),
            payload: payload,
        };
        println!("Reply: {:?}", reply);

        println!("################")

        // TODO: implement me
        // 1. Create the event and payload
        // 2. Submit the the RequestReplyRequest to the channel
        // 2.1 Are we using queues or channels?
        // 3. Wait for the response

    }


}

async fn test_send_smoke(task_id: u8, delay_millis: u64, event_service: Arc<EventServiceGrpc>) {
    let mut counter = 0;
    loop {
        // convert counter to Message
        let mut counter_msg = Int32Value::new();
        counter_msg.value = counter;

        event_service.publish(
            String::from(format!("test/{}", task_id)),
            counter_msg,
            None,
            false,
        ).await;

        tokio::time::sleep(Duration::from_millis(delay_millis)).await;
        println!("task_id: {}, counter: {}", task_id, counter);
        counter += 1;
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr= "[::1]:50051".parse()?;

    // creating a service
    let event_service = EventServiceGrpc::default();
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
