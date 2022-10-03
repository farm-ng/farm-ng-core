#[[
sophus_serial



]]
farm_ng_add_library(farm_ng_core_prototools
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    proto_reader_writer.h
  SOURCES
    proto_reader_writer.cpp

)

target_link_libraries(farm_ng_core_prototools protobuf::libprotobuf farm_ng_core_logging)


foreach(test_basename proto_reader_writer)
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY farm_ng_core_prototools
                          LINK_LIBRARIES farm_ng_core_prototools
                          LABELS small)
endforeach()
