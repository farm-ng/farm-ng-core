farm_ng_add_library(farm_ng_core_proto_conv_lie
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_lie PUBLIC
  farm_ng_core::sophus2_lie
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_proto_conv_std
  farm_ng_core::farm_ng_core_proto_conv_linalg
  farm_ng_core::farm_ng_core_prototools
)

farm_ng_add_test(conv
              PARENT_LIBRARY farm_ng_core_proto_conv_lie
              LINK_LIBRARIES farm_ng_core_proto_conv_lie
              LABELS small)
