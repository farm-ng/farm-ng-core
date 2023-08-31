farm_ng_add_library(farm_ng_core_proto_conv_sensor
NAMESPACE farm_ng_core
INCLUDE_DIR ../../..
HEADERS
  conv.h
SOURCES
  conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_sensor PUBLIC
  Sophus::sophus_sensor
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_prototools
  farm_ng_core::farm_ng_core_proto_conv_image
)


farm_ng_add_test(conv
              PARENT_LIBRARY farm_ng_core_proto_conv_sensor
              LINK_LIBRARIES farm_ng_core_proto_conv_sensor
              LABELS small)
