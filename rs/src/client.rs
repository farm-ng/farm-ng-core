use farm_ng::core::event_service_client::EventServiceClient;
use farm_ng::core::ListUrisRequest;

pub mod farm_ng {
    pub mod core {
        tonic::include_proto!("farm_ng.core.proto");
    }
}


#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut client = EventServiceClient::connect("http://[::1]:50051").await?;

    let request = tonic::Request::new(ListUrisRequest {});

    let response = client.list_uris(request).await?;

    println!("RESPONSE={:?}", response);

    Ok(())
}
