farm_ng_add_library(farm_ng_core_misc_proto_conv
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_misc_proto_conv PUBLIC protobuf::libprotobuf farm_ng_core_prototools farm_ng_core_proto_defs)

farm_ng_add_test(conv
                PARENT_LIBRARY farm_ng_core_misc_proto_conv
                LINK_LIBRARIES farm_ng_core_misc_proto_conv
                LABELS small)
