set(farm_ng_core_misc_proto_conv
  conv
)

 set(farm_ng_core_misc_proto_conv_cpp)
set(farm_ng_core_misc_proto_conv_h)
foreach(x ${farm_ng_core_misc_proto_conv} )
    list(APPEND farm_ng_core_misc_proto_conv_cpp ${x}.cpp)
    list(APPEND farm_ng_core_misc_proto_conv_h ${x}.h)
endforeach()

 add_library(farm_ng_core_misc_proto_conv SHARED
  ${farm_ng_core_misc_proto_conv_cpp}
  ${farm_ng_core_misc_proto_conv_h})
target_link_libraries(farm_ng_core_misc_proto_conv protobuf::libprotobuf farm_ng_core_prototools farm_ng_core_proto_defs)
add_library (farm_ng_core::farm_ng_core_misc_proto_conv ALIAS farm_ng_core_misc_proto_conv)
target_include_directories(farm_ng_core_misc_proto_conv PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(farm_ng_core_misc_proto_conv INTERFACE
              FILE_SET HEADERS
              BASE_DIRS ../../../..
              FILES ${farm_ng_core_misc_proto_conv_h})

if(${FARM_NG_CORE_TESTS})
  farm_ng_add_test(conv
                PARENT_LIBRARY farm_ng_core_misc_proto_conv
                LINK_LIBRARIES
                LABELS small)
endif()
