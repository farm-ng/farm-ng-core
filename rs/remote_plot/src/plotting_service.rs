use crate::grpc::farm_ng::core::plotting::proto as grpc_protos;

// sender used by the service
pub type UpdateSender = tokio::sync::mpsc::Sender< crate::message::UpdateMessages >;
// receiver used by the app
pub type UpdateReceiver = tokio::sync::mpsc::Receiver<crate::message::UpdateMessages >;

/// Service implementation
#[derive(Clone, Debug)]
pub struct PlottingService {
    update_sender: UpdateSender,
}

impl PlottingService {
    /// Create a new service with the given sender
    pub fn new(update_sender: UpdateSender) -> Self {
        PlottingService { update_sender }
    }
}

#[tonic::async_trait]
impl grpc_protos::plotting_widget_server::PlottingWidget for PlottingService {

    /// Forwards set_x_range requests to the app
    async fn send(
        &self,
        request: tonic::Request<grpc_protos::Messages>,
    ) -> std::result::Result<tonic::Response<grpc_protos::PlottingReply>, tonic::Status> {
        let messages: grpc_protos::Messages = request.into_inner();
        match self
            .update_sender
            .send(crate::message::UpdateMessages{proto_messages:messages})
            .await
        {
            Ok(_) => {}
            Err(e) => {
                return Result::Err(tonic::Status::internal(e.to_string()));
            }
        }
        Ok(tonic::Response::new(grpc_protos::PlottingReply {}))
    }
}
