use farm_ng::core::event_service_client::EventServiceClient;
use tonic::transport::Channel;
use tokio_stream::StreamExt;

use protobuf::Message;
use protobuf::well_known_types::wrappers::{Int32Value, StringValue};

pub mod farm_ng {
    pub mod core {
        tonic::include_proto!("farm_ng.core.proto");
    }
}

async fn streaming_subscribe(client: &mut EventServiceClient<Channel>) {
    let mut stream = client
        .subscribe(farm_ng::core::SubscribeRequest {
            uri: Some(farm_ng::core::Uri {
                scheme: "proto".to_string(),
                authority: "farm_ng".to_string(),
                path: "foo".to_string(),
                query: "bar".to_string(),
            }),
            every_n: 1,
        })
        .await
        .unwrap()
        .into_inner();

    // stream is infinite - take just 5 elements and then disconnect
    // let mut stream = stream.take(5);
    while let Some(response) = stream.next().await {
        // decode the bytes into a protobuf message
        //let decoded: Int32Value = Message::parse_from_bytes(&response.unwrap().payload).unwrap();
        let decoded: StringValue = Message::parse_from_bytes(&response.unwrap().payload).unwrap();
        println!("RESPONSE={:?}", decoded)
    }

}


#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut client = EventServiceClient::connect("http://[::1]:50051").await?;

    //let request = tonic::Request::new(ListUrisRequest {});
    //let response = client.list_uris(request).await?;
    //println!("RESPONSE={:?}", response);

    streaming_subscribe(&mut client).await;

    Ok(())
}
