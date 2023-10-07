// use crate::{grpc::farm_ng::core::plotting::proto as grpc_protos, actors::plotter::PlotterMessage};

use std::sync::Arc;

use async_trait::async_trait;

use hollywood::compute::*;
use hollywood::core::actor::ActorNode;
use hollywood::core::actor::DormantActorNode;
use hollywood::core::inbound::ForwardMessage;
use hollywood::core::runner::Runner;
use hollywood::core::*;
use hollywood::macros::*;

use crate::graphs::packets::PlottingPackets;
use crate::grpc::farm_ng::core::plotting::proto::plotting_widget_server::PlottingWidget;
use crate::grpc::farm_ng::core::plotting::proto::plotting_widget_server::PlottingWidgetServer;
use crate::grpc::farm_ng::core::plotting::proto::Messages;
use crate::grpc::farm_ng::core::plotting::proto::PlottingReply;
use crate::grpc::proto::from_proto;

/// Outbound hub
#[actor_outputs]
pub struct GrpcSourceOutbound {
    pub plotter_message: OutboundChannel<PlottingPackets>,
}

#[derive(Clone, Debug, Default)]
pub struct GrpcSourceProp {
    pub port: u32,
}

impl Value for GrpcSourceProp {}

pub struct GrpcSourceActor {
    /// unique identifier of the actor
    pub actor_name: String,
}

impl
    FromPropState<
        GrpcSourceProp,
        NullInbound,
        NullState,
        GrpcSourceOutbound,
        NullMessage<GrpcSourceProp, NullState, GrpcSourceOutbound>,
        GrpcSourceRunner,
    > for GrpcSourceActor
{
    fn name_hint(_prop: &GrpcSourceProp) -> String {
        "Plotter".to_owned()
    }
}

/// The custom runner for the plotter actor.
pub struct GrpcSourceRunner {}

impl
    Runner<
        GrpcSourceProp,
        NullInbound,
        NullState,
        GrpcSourceOutbound,
        NullMessage<GrpcSourceProp, NullState, GrpcSourceOutbound>,
    > for GrpcSourceRunner
{
    /// Create a new dormant actor.
    fn new_dormant_actor(
        name: String,
        prop: GrpcSourceProp,
        _state: NullState,
        _receiver: tokio::sync::mpsc::Receiver<
            NullMessage<GrpcSourceProp, NullState, GrpcSourceOutbound>,
        >,
        _forward: std::collections::HashMap<
            String,
            Box<
                dyn ForwardMessage<
                        GrpcSourceProp,
                        NullState,
                        GrpcSourceOutbound,
                        NullMessage<GrpcSourceProp, NullState, GrpcSourceOutbound>,
                    > + Send
                    + Sync,
            >,
        >,
        outbound: GrpcSourceOutbound,
    ) -> Box<dyn DormantActorNode + Send + Sync> {
        Box::new(DormantGrpcSourceActor {
            name: name.clone(),
            prop,
            outbound,
        })
    }
}

/// The dormant plotter actor.
pub struct DormantGrpcSourceActor {
    name: String,
    prop: GrpcSourceProp,
    outbound: GrpcSourceOutbound,
}

impl DormantActorNode for DormantGrpcSourceActor {
    fn activate(mut self: Box<Self>) -> Box<dyn ActorNode + Send> {
        self.outbound.activate();

        Box::new(ActiveGrpcSourceNodeImpl {
            name: self.name.clone(),
            prop: self.prop,
            outbound: Arc::new(self.outbound),
        })
    }
}

pub(crate) struct ActiveGrpcSourceNodeImpl {
    pub(crate) name: String,
    prop: GrpcSourceProp,
    outbound: Arc<GrpcSourceOutbound>,
}

#[async_trait]
impl ActorNode for ActiveGrpcSourceNodeImpl {
    fn name(&self) -> &String {
        &self.name
    }

    fn reset(&mut self) {
        // no-op
    }

    async fn run(&mut self, mut _kill: tokio::sync::broadcast::Receiver<()>) {
        let address: String = format!("0.0.0.0:{}", self.prop.port);

        let server = PlottingService::new(self.outbound.clone());
        let server_future = tonic::transport::Server::builder()
            .add_service(PlottingWidgetServer::new(server))
            .serve(address.parse().unwrap());
        match server_future.await {
            Ok(_) =>{}
            Err(e) =>{panic!("{}", e);}
        }
    }
}

struct PlottingService {
    outbound: Arc<GrpcSourceOutbound>,
}

impl PlottingService {
    /// Create a new service with the given sender
    pub fn new(outbound: Arc<GrpcSourceOutbound>) -> Self {
        PlottingService { outbound }
    }
}

#[async_trait]
impl PlottingWidget for PlottingService {
    async fn send(
        &self,
        request: tonic::Request<Messages>,
    ) -> std::result::Result<tonic::Response<PlottingReply>, tonic::Status> {
        let proto_messages: Messages = request.into_inner();

        let packets = from_proto(proto_messages);

        println!("Got {} messages", packets.len());

        self.outbound.as_ref().plotter_message.send(packets);

        Ok(tonic::Response::new(PlottingReply {}))
    }
}
