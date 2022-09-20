#[[
sophus_serial



]]

set(farm_ng_core_protobuf_src_prefixes
  proto_reader_writer
)

set(farm_ng_core_protobuf_cpp)
set(farm_ng_core_protobuf_h)
foreach(x ${farm_ng_core_protobuf_src_prefixes} )
    list(APPEND farm_ng_core_protobuf_cpp ${x}.cpp)
    list(APPEND farm_ng_core_protobuf_h ${x}.h)
endforeach()

add_library(farm_ng_core_protobuf SHARED
  ${farm_ng_core_protobuf_cpp}
  ${farm_ng_core_protobuf_h})
target_link_libraries(farm_ng_core_protobuf protobuf::libprotobuf farm_ng_core_logging)
add_library (farm_ng_core::farm_ng_core_protobuf ALIAS farm_ng_core_protobuf)
target_include_directories(farm_ng_core_protobuf PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(farm_ng_core_protobuf INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../../..
               FILES ${farm_ng_core_protobuf_h})

if(${BUILD_SOPHUS_TESTS})
  foreach(test_basename ${farm_ng_core_protobuf_src_prefixes})
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY farm_ng_core_protobuf
                          LINK_LIBRARIES
                          LABELS small)
  endforeach()
endif()
