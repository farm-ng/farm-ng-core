farm_ng_add_library(farm_ng_core_proto_conv_color
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_color PUBLIC
  farm_ng_core::sophus_color
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_prototools
  farm_ng_core_proto_defs
  farm_ng_core::farm_ng_core_proto_conv_linalg)


farm_ng_add_test(conv
              PARENT_LIBRARY farm_ng_core_proto_conv_color
              LINK_LIBRARIES farm_ng_core_proto_conv_color
              LABELS small)
