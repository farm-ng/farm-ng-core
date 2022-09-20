#[[
farm_ng_core_enum

Set of macros for compile-time enum introspection / autogeneration of auxilary
helper functions.

Depends on farm_ng_core_logging.

]]

add_library(farm_ng_core_enum INTERFACE
  enum.h
  enum_flags.h
  enum_without_ostream.h
  enum_flags_without_ostream.h)

target_include_directories(farm_ng_core_enum INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_sources(farm_ng_core_enum INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../../..
               FILES
                 enum.h
                 enum_flags.h
                 enum_without_ostream.h
                 enum_flags_without_ostream.h
                 impl/enum_details.h
                 impl/enum_flag_details.h)

if (${FARM_NG_CORE_TESTS})
  set(FARM_NG_ENUM_TESTS
      enum
      enum_without_ostream
      enum_flags
      enum_flags_without_ostream)

  foreach(test_basename ${FARM_NG_ENUM_TESTS})
    farm_ng_add_test(${test_basename}
                      PARENT_LIBRARY farm_ng_core_enum
                      LINK_LIBRARIES farm_ng_core_logging
                      LABELS small)
  endforeach()
endif()
