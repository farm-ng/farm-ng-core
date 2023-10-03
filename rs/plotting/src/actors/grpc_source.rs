// use crate::{grpc::farm_ng::core::plotting::proto as grpc_protos, actors::plotter::PlotterMessage};

use std::mem::size_of;
use std::sync::Arc;

use async_trait::async_trait;

use hollywood::compute::*;
use hollywood::core::actor::ActorNode;
use hollywood::core::actor::DormantActorNode;
use hollywood::core::inbound::ForwardMessage;
use hollywood::core::runner::Runner;
use hollywood::core::*;
use hollywood::macros::*;

use crate::graphs::scalar_curve::NamedScalarCurve;
use crate::graphs::scalar_curve::ScalarCurve;
use crate::grpc::farm_ng::core::plotting::proto;
use crate::grpc::farm_ng::core::plotting::proto::plotting_widget_server::PlottingWidget;
use crate::grpc::farm_ng::core::plotting::proto::plotting_widget_server::PlottingWidgetServer;
use crate::grpc::farm_ng::core::plotting::proto::Curve;
use crate::grpc::farm_ng::core::plotting::proto::Messages;
use crate::grpc::farm_ng::core::plotting::proto::PlottingReply;

use super::plotter::PlottingPacket;
use super::plotter::PlottingPackets;

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
FromNewWithState<
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

    async fn run(&mut self, mut kill: tokio::sync::broadcast::Receiver<()>) {
        let address: String = format!("0.0.0.0:{}", self.prop.port);

        let server = PlottingService::new(self.outbound.clone());
        let server_future = tonic::transport::Server::builder()
            .add_service(PlottingWidgetServer::new(server))
            .serve(address.parse().unwrap());
        server_future.await.unwrap();
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

fn color_from_proto(
    proto: crate::grpc::farm_ng::core::proto::Color,
) -> crate::graphs::common::Color {
    crate::graphs::common::Color {
        r: proto.r,
        g: proto.g,
        b: proto.b,
        a: proto.a,
    }
}

fn line_type_from_proto(proto: proto::LineType) -> String {
    proto.variant_name
}

fn from_proto(m: Messages) -> PlottingPackets {
    let mut p = vec![];
    for mes in m.messages {
        match mes.variant {
            Some(v) => match v {
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Curve(
                    proto_curve,
                ) => {
                    let path: String = proto_curve.path.unwrap().path_string;
                    let tokens = path.split("/").collect::<Vec<&str>>();
                    assert_eq!(
                        tokens.len(),
                        2,
                        "path must be of the form `plot_name/curve_name`, got '{}'",
                        path
                    );
                    let plot_name = tokens[0];
                    let curve_name = tokens[1];

                    let pairs_as_f64: Vec<(f64, f64)> = proto_curve
                        .x_y_pairs
                        .unwrap()
                        .data
                        .chunks_exact(size_of::<f64>() * 2)
                        .map(|b| {
                            (
                                f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                                f64::from_ne_bytes(b[size_of::<f64>()..].try_into().unwrap()),
                            )
                        })
                        .collect();

                    p.push(PlottingPacket::ScalarCurve(NamedScalarCurve {
                        plot_name: plot_name.to_owned(),
                        graph_name: curve_name.to_owned(),
                        scalar_curve: ScalarCurve {
                            data: pairs_as_f64,
                            color: color_from_proto(proto_curve.color.unwrap()),
                            curve_type: line_type_from_proto(proto_curve.line_type.unwrap()),
                        },
                    }));
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Vec3Curve(_) => {
                    todo!()
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Vec3ConfCurve(_) => {
                    todo!()
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Rects(_) => todo!(),
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::XRange(_) => todo!(),
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::YRange(_) => todo!(),
            },
            None => todo!(),
        }
    }
    return p;
}

#[async_trait]
impl PlottingWidget for PlottingService {
    /// Forwards set_x_range requests to the app
    async fn send(
        &self,
        request: tonic::Request<Messages>,
    ) -> std::result::Result<tonic::Response<PlottingReply>, tonic::Status> {
        let proto_messages: Messages = request.into_inner();

        let packets = from_proto(proto_messages);

        self.outbound.as_ref().plotter_message.send(packets);

        Ok(tonic::Response::new(PlottingReply {}))
    }
}
