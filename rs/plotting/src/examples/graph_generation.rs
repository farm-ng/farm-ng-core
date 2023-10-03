use hollywood::macros::*;
pub use hollywood::compute::Context;
pub use hollywood::core::*;

// needed for actor macro
pub use hollywood::core::{Actor, ActorFacade, DefaultRunner};

use crate::actors::plotter::PlottingPackets;
use crate::graphs::scalar_curve::NamedScalarCurve;

/// Outbound hub for the GraphGenerator.
#[actor_outputs]
pub struct GraphGeneratorOutbound {
    /// sin curve
    pub packets: OutboundChannel<PlottingPackets>,

    /// cancel request
    pub cancel_request: OutboundChannel<()>,
}

/// Properties of the GraphGenerator actor.
#[derive(Clone, Debug)]
pub struct GraphGeneratorProp {
    /// Time when cancel request is send out.
    pub timeout: f64,
}

impl Default for GraphGeneratorProp {
    fn default() -> Self {
        GraphGeneratorProp { timeout: 10.0 }
    }
}

impl Value for GraphGeneratorProp {}


/// Inbound message for the GraphGenerator actor.
///
#[derive(Clone, Debug)]
#[actor_inputs(GraphGeneratorInbound, {GraphGeneratorProp, NullState, GraphGeneratorOutbound})]
pub enum GraphGeneratorMessage {
    /// in seconds
    ClockTick(f64),
}

impl OnMessage for GraphGeneratorMessage {
    /// Process the inbound time_stamp message.
    fn on_message(&self, prop: &Self::Prop, _state: &mut Self::State, outbound: &Self::OutboundHub) {
        match &self {
            GraphGeneratorMessage::ClockTick(time_in_seconds) => {
                let curve = NamedScalarCurve {
                    plot_name: "graphs".to_string(),
                    graph_name: "sin".to_string(),
                    scalar_curve: crate::graphs::scalar_curve::ScalarCurve {
                        data: vec![(*time_in_seconds, time_in_seconds.sin())],
                        color: crate::graphs::common::Color::red(),
                        curve_type: "line_strip".to_string(),
                    },
                };

                let packets = vec![crate::actors::plotter::PlottingPacket::ScalarCurve(curve)];
                outbound.packets.send(packets);

                if time_in_seconds > &prop.timeout {
                    outbound.cancel_request.send(());
                }
            }
        }
    }
}

impl InboundMessageNew<f64> for GraphGeneratorMessage {
    fn new(_inbound_name: String, msg: f64) -> Self {
        GraphGeneratorMessage::ClockTick(msg)
    }
}

/// The GraphGenerator actor.
///
#[actor(GraphGeneratorMessage)]
type GraphGenerator = Actor<
    GraphGeneratorProp,
    GraphGeneratorInbound,
    NullState,
    GraphGeneratorOutbound,
>;
