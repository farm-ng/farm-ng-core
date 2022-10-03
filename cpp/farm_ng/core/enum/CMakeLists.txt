#[[
farm_ng_core_enum

Set of macros for compile-time enum introspection / autogeneration of auxilary
helper functions.

Depends on farm_ng_core_logging.

]]
farm_ng_add_library(farm_ng_core_enum
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    enum.h
    enum_flags.h
    enum_without_ostream.h
    enum_flags_without_ostream.h
    enum.h
    enum_flags.h
    enum_without_ostream.h
    enum_flags_without_ostream.h
    impl/enum_details.h
    impl/enum_flag_details.h
)

target_link_libraries(farm_ng_core_enum INTERFACE farm_ng_core_logging)

set(FARM_NG_ENUM_TESTS
      enum
      enum_without_ostream
      enum_flags
      enum_flags_without_ostream)

foreach(test_basename ${FARM_NG_ENUM_TESTS})
    farm_ng_add_test(${test_basename}
                      PARENT_LIBRARY farm_ng_core_enum
                      LINK_LIBRARIES farm_ng_core_enum
                      LABELS small)
endforeach()
