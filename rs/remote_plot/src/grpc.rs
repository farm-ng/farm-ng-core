pub mod farm_ng {
    pub mod core {

        pub mod proto {

            include!(concat!(env!("OUT_DIR"), "/farm_ng.core.proto.rs"));
        }

        pub mod plotting {
            pub mod proto {

                include!(concat!(env!("OUT_DIR"), "/farm_ng.core.plotting.proto.rs"));
            }
        }
    }
}
