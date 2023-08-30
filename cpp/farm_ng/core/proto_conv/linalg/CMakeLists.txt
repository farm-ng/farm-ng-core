farm_ng_add_library(farm_ng_core_proto_conv_linalg
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_linalg PUBLIC
  Sophus::sophus_linalg
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_proto_defs
  farm_ng_core::farm_ng_core_prototools
 )

if(${BUILD_SOPHUS_TESTS})
  farm_ng_add_test(conv
                PARENT_LIBRARY farm_ng_core_proto_conv_linalg
                LINK_LIBRARIES farm_ng_core_proto_conv_linalg
                LABELS small)
endif()
