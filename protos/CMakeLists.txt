farm_ng_add_protobufs(farm_ng_core_proto_defs
NAMESPACE farm_ng_core
PROTO_FILES
./farm_ng/core/event.proto
./farm_ng/core/timestamp.proto
./farm_ng/core/uri.proto
./farm_ng/core/linalg.proto
./farm_ng/core/lie.proto
./farm_ng/core/pose.proto
./farm_ng/core/image.proto)

farm_ng_add_protobufs(sophus_linalg_proto_defs
    NAMESPACE farm_ng_core
    PROTO_FILES
    sophus/linalg.proto)

farm_ng_add_protobufs(sophus_lie_proto_defs
      NAMESPACE farm_ng_core
      PROTO_FILES
      sophus/lie.proto
      DEPENDENCIES
      farm_ng_core::sophus_linalg_proto_defs)

farm_ng_add_protobufs(sophus_geometry_proto_defs
      NAMESPACE farm_ng_core
      PROTO_FILES
      sophus/geometry.proto
      DEPENDENCIES
      farm_ng_core::sophus_linalg_proto_defs)

farm_ng_add_protobufs(sophus_image_proto_defs
      NAMESPACE farm_ng_core
      PROTO_FILES
      sophus/image.proto
      DEPENDENCIES
      farm_ng_core::sophus_lie_proto_defs
      farm_ng_core::sophus_geometry_proto_defs)

farm_ng_add_protobufs(sophus_sensor_proto_defs
      NAMESPACE farm_ng_core
      PROTO_FILES
      sophus/sensor.proto
      DEPENDENCIES
      farm_ng_core::sophus_image_proto_defs)
