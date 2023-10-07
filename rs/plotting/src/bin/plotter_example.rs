pub use hollywood::compute::Context;
pub use hollywood::core::*;
use hollywood::macros::*;

use plotting::actors::plotter::{run_on_main_thread, PlotterActor, PlotterProp, PlotterState};
use plotting::graphs::packets::PlottingPacket;
use plotting::graphs::packets::PlottingPackets;

/// The GraphGenerator actor.
///
#[actor(GraphGeneratorMessage)]
type GraphGenerator = Actor<NullProp, GraphGeneratorInbound, NullState, GraphGeneratorOutbound>;

/// Inbound message for the GraphGenerator actor.
///
#[derive(Clone, Debug)]
#[actor_inputs(GraphGeneratorInbound, {NullProp, NullState, GraphGeneratorOutbound})]
pub enum GraphGeneratorMessage {
    /// in seconds
    ClockTick(f64),
}

/// Outbound hub for the GraphGenerator.
#[actor_outputs]
pub struct GraphGeneratorOutbound {
    /// curves
    pub packets: OutboundChannel<PlottingPackets>,
}

impl OnMessage for GraphGeneratorMessage {
    /// Process the inbound time_stamp message.
    fn on_message(
        &self,
        _prop: &Self::Prop,
        _state: &mut Self::State,
        outbound: &Self::OutboundHub,
    ) {
        match &self {
            GraphGeneratorMessage::ClockTick(time_in_seconds) => {
                let mut packets = vec![];
                packets.push(PlottingPacket::append_to_curve(
                    ("graph", "sin"),
                    plotting::graphs::common::Color::red(),
                    (*time_in_seconds, time_in_seconds.sin()),
                    10.0,
                ));
                packets.push(PlottingPacket::append_to_vec3_curve(
                    ("graph2", "sins"),
                    (
                        *time_in_seconds,
                        (
                            time_in_seconds.cos(),
                            (2.0 * time_in_seconds).cos(),
                            (3.0 * time_in_seconds).cos(),
                        ),
                    ),
                    10.0,
                ));
                outbound.packets.send(packets);
            }
        }
    }
}

impl InboundMessageNew<f64> for GraphGeneratorMessage {
    fn new(_inbound_name: String, msg: f64) -> Self {
        GraphGeneratorMessage::ClockTick(msg)
    }
}

pub async fn run_graph_plotting_example() {
    let (update_sender, update_receiver) = std::sync::mpsc::channel();

    let pipeline = hollywood::compute::Context::configure(&mut |context| {
        let mut timer = hollywood::actors::Periodic::new_with_period(context, 0.01);

        let mut graph_generator =
            GraphGenerator::from_prop_and_state(context, NullProp {}, NullState {});

        let mut plotter = PlotterActor::from_prop_and_state(
            context,
            PlotterProp {},
            PlotterState::new(update_sender.clone()),
        );

        timer
            .outbound
            .time_stamp
            .connect(context, &mut graph_generator.inbound.clock_tick);

        graph_generator
            .outbound
            .packets
            .connect(context, &mut plotter.inbound.packets);
    });

    pipeline.print_flow_graph();
    let cancel_requester = pipeline.cancel_request_sender_template.clone().unwrap();
    let pipeline_handle = tokio::spawn(pipeline.run());

    run_on_main_thread(update_receiver, cancel_requester);

    pipeline_handle.await.unwrap();
}

fn main() {
    tokio::runtime::Builder::new_multi_thread()
        .enable_all()
        .build()
        .unwrap()
        .block_on(async {
            run_graph_plotting_example().await;
        })
}
