use hollywood::core::ActorFacade;
use plotting::actors::plotter::{
    run_on_main_thread, PlotterActor, PlotterMessage, PlotterProp, PlotterState,
};
use plotting::examples::graph_generation::{GraphGenerator, GraphGeneratorProp};

pub async fn run_graph_plotting_example() {
    let (update_sender, update_receiver) = std::sync::mpsc::channel();

    let pipeline = hollywood::compute::Context::configure(&mut |context| {
        let mut timer = hollywood::actors::Periodic::new_with_period(context, 0.01);

        let mut graph_generator =
            GraphGenerator::new_default_init_state(context, GraphGeneratorProp { timeout: 10.0 });

        let mut plotter = PlotterActor::new_with_state(
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

        context.register_cancel_requester(&mut graph_generator.outbound.cancel_request);
    });

    pipeline.print_flow_graph();
    let pipeline_handle = tokio::spawn(pipeline.run());

    run_on_main_thread(update_receiver);

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
