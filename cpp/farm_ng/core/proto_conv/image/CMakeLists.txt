farm_ng_add_library(farm_ng_core_proto_conv_image
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    conv.h
  SOURCES
    conv.cpp
)

target_link_libraries(farm_ng_core_proto_conv_image PUBLIC
  Sophus::sophus_linalg
  protobuf::libprotobuf
  farm_ng_core::farm_ng_core_proto_defs
  farm_ng_core::farm_ng_core_prototools
  farm_ng_core::farm_ng_core_proto_conv_linalg
  sophus_image
 )

if(${BUILD_SOPHUS_TESTS})
  farm_ng_add_test(conv
                PARENT_LIBRARY farm_ng_core_proto_conv_image
                LINK_LIBRARIES farm_ng_core_proto_conv_image
                LABELS small)
endif()
