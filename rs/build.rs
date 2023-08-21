fn main() -> Result<(), Box<dyn std::error::Error>> {
    tonic_build::configure()
        .build_server(true)
        .compile(
            &[
                "../protos/farm_ng/core/event.proto",
                "../protos/farm_ng/core/event_service.proto",
                "../protos/farm_ng/core/timestamp.proto",
                "../protos/farm_ng/core/uri.proto",
            ],
            &["../protos"],
        )?;
    Ok(())
}
