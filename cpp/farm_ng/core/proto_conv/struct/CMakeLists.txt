farm_ng_add_library(farm_ng_core_proto_conv_struct
NAMESPACE farm_ng_core
INCLUDE_DIR ../../..
HEADERS
  conv.h
SOURCES
)

target_link_libraries(farm_ng_core_proto_conv_struct INTERFACE
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_struct
  farm_ng_core::farm_ng_core_prototools
)


farm_ng_add_test(conv
              PARENT_LIBRARY farm_ng_core_proto_conv_struct
              LINK_LIBRARIES farm_ng_core_proto_conv_struct
              LABELS small)
