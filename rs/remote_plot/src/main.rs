use clap::Parser;
use remote_plot::grpc::farm_ng::core::plotting::proto as grpc_protos;

#[derive(Parser, Debug)]
struct Args {
    #[arg(short, long, default_value_t = 1980)]
    port: u32,
}

async fn run() -> Result<(), Box<dyn std::error::Error>> {
    // Create a channel to send updates from the gRPC server to the plotting app
    let (update_sender, update_receiver) =
        tokio::sync::mpsc::channel::<remote_plot::message::UpdateMessages>(100);

    // Parse command line arguments
    let args: Args = Args::parse();
    let port: u32 = args.port;
    let address: String = format!("0.0.0.0:{port}");

    // Create the gRPC server and spawn it in a separate tokio task
    let server = remote_plot::plotting_service::PlottingService::new(update_sender);
    let server_future = tonic::transport::Server::builder()
        .add_service(grpc_protos::plotting_widget_server::PlottingWidgetServer::new(server))
        .serve(address.parse()?);
    let server_task = tokio::spawn(server_future);

    // Create the plotting app and run it
    let gui = remote_plot::plot_gui::PlottingApp::new(update_receiver);
    let options = eframe::NativeOptions::default();
    eframe::run_native(
        "Remote Plotting Service",
        options,
        Box::new(|_cc| Box::new(gui)),
    )?;

    // Wait for the server task to finish (if it ever does)
    server_task.await??;

    Ok(())
}


fn main() {
    tokio::runtime::Builder::new_multi_thread()
        .enable_all()
        .build()
        .unwrap()
        .block_on(async {
            run().await.unwrap();
        })
}
