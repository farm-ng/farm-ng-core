farm_ng_add_library(farm_ng_core_prototools
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    proto_reader_writer.h
    event_log_writer.h
    event_log_reader.h
    event_service.h
    event_and_payload.h
  SOURCES
    proto_reader_writer.cpp
    event_log_writer.cpp
    event_log_reader.cpp
    event_service.cpp
    event_and_payload.cpp
)

target_link_libraries(farm_ng_core_prototools protobuf::libprotobuf farm_ng_core_logging farm_ng_core_proto_defs farm_ng_core_pipeline)


foreach(test_basename proto_reader_writer event_log_writer event_service)
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY farm_ng_core_prototools
                          LINK_LIBRARIES farm_ng_core_prototools farm_ng_core_misc
                          LABELS small)
endforeach()


add_executable(farm_ng_event_service
main_event_service.cpp
)
target_link_libraries(farm_ng_event_service farm_ng_core_prototools)
install(
  TARGETS farm_ng_event_service
  RUNTIME DESTINATION bin
)
