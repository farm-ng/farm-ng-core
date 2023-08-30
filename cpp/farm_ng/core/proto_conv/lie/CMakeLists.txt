farm_ng_add_library(farm_ng_core_proto_conv_lie
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_lie PUBLIC
  Sophus::sophus_lie
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_proto_conv_linalg
  farm_ng_core::farm_ng_core_prototools
)
