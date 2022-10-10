#[[
sophus_serial



]]
farm_ng_add_library(farm_ng_core_prototools
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    proto_reader_writer.h
    event_log_writer.h
    event_log_reader.h
  SOURCES
    proto_reader_writer.cpp
    event_log_writer.cpp
    event_log_reader.cpp
)

target_link_libraries(farm_ng_core_prototools protobuf::libprotobuf farm_ng_core_logging farm_ng_core_proto_defs)


foreach(test_basename proto_reader_writer event_log_writer)
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY farm_ng_core_prototools
                          LINK_LIBRARIES farm_ng_core_prototools farm_ng_core_misc
                          LABELS small)
endforeach()
