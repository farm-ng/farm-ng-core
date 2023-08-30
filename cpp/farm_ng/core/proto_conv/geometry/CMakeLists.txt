farm_ng_add_library(farm_ng_core_proto_conv_geometry
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_geometry PUBLIC
  Sophus::sophus_geometry
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_prototools
  farm_ng_core_proto_defs
  farm_ng_core::farm_ng_core_proto_conv_linalg)
