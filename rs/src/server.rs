use std::{pin::Pin, time::Duration};
use tokio::sync::mpsc;
use tokio_stream::{wrappers::ReceiverStream, Stream, StreamExt};
use tonic::{transport::Server, Request, Response, Status};

use farm_ng::core::{
    event_service_server::{EventService, EventServiceServer},
    ListUrisReply, ListUrisRequest,
    RequestReplyReply, RequestReplyRequest,
    SubscribeReply, SubscribeRequest,
    Uri,
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

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr = "[::1]:50051".parse()?;

    // creating a service
    let server = EventServiceGrpc::default();
    println!("Server listening on {}", addr);

    // adding our service to our server.
    Server::builder()
        .add_service(EventServiceServer::new(server))
        .serve(addr)
        .await?;
    Ok(())
}
