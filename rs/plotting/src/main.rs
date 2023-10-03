use clap::Parser;

#[derive(Parser, Debug)]
struct Args {
    #[arg(short, long, default_value_t = 1980)]
    port: u32,
}

async fn run() -> Result<(), Box<dyn std::error::Error>> {
    // // Create a channel to send updates from the gRPC server to the plotting app
    // let (update_sender, update_receiver) = std::sync::mpsc::channel();

    // // Parse command line arguments
    // let args: Args = Args::parse();
    // let port: u32 = args.port;
    // let address: String = format!("0.0.0.0:{port}");

    // // Create the gRPC server and spawn it in a separate tokio task
    // let server = plotting::plotting_service::PlottingService::new(update_sender);
    // let server_future = tonic::transport::Server::builder()
    //     .add_service(grpc_protos::plotting_widget_server::PlottingWidgetServer::new(server))
    //     .serve(address.parse()?);
    // let server_task = tokio::spawn(server_future);

    // // Create the plotting app and run it
    // let gui = plotting::plot_gui::PlottingApp::new(update_receiver);
    // let options = eframe::NativeOptions::default();
    // eframe::run_native(
    //     "Remote Plotting Service",
    //     options,
    //     Box::new(|_cc| Box::new(gui)),
    // )?;

    // // Wait for the server task to finish (if it ever does)
    // server_task.await??;

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
