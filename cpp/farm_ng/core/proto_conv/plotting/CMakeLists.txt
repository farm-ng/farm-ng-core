farm_ng_add_library(farm_ng_core_proto_conv_plotting
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_plotting PUBLIC
  farm_ng_core::sophus_color
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_proto_defs
  farm_ng_core::farm_ng_core_proto_conv_color
  farm_ng_core::farm_ng_core_proto_conv_calculus
  farm_ng_core::farm_ng_core_proto_conv_std
)

farm_ng_add_test(conv
              PARENT_LIBRARY farm_ng_core_proto_conv_plotting
              LINK_LIBRARIES farm_ng_core_proto_conv_plotting
              LABELS small)
