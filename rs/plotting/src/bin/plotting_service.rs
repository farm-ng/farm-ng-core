pub use hollywood::compute::Context;
pub use hollywood::core::*;
use plotting::actors::grpc_source::{GrpcSourceActor, GrpcSourceProp};
use plotting::actors::plotter::{run_on_main_thread, PlotterActor, PlotterProp, PlotterState};

pub async fn run_graph_plotting_example() {
    let (update_sender, update_receiver) = std::sync::mpsc::channel();

    let pipeline = hollywood::compute::Context::configure(&mut |context| {
        let mut service = GrpcSourceActor::from_prop_and_state(
            context,
            GrpcSourceProp { port: 1980 },
            NullState {},
        );

        let mut plotter = PlotterActor::from_prop_and_state(
            context,
            PlotterProp {},
            PlotterState::new(update_sender.clone()),
        );

        service
            .outbound
            .plotter_message
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
