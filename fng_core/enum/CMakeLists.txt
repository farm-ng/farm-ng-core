
add_library(fng_core_enum INTERFACE
  enum.h
  enum_flags.h
  enum_without_ostream.h
  enum_flags_without_ostream.h)

target_include_directories(fng_core_enum INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_sources(fng_core_enum INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../..
               FILES
                 enum.h
                 enum_flags.h
                 enum_without_ostream.h
                 enum_flags_without_ostream.h
                 impl/enum_details.h
                 impl/enum_flag_details.h)

if (${FNG_CORE_TESTS})
  set(FNG_ENUM_TESTS
      enum
      enum_without_ostream
      enum_flags
      enum_flags_without_ostream)

  foreach(test_basename ${FNG_ENUM_TESTS})
    fng_core_add_test(${test_basename}
                      PARENT_LIBRARY fng_core_enum
                      LINK_LIBRARIES fng_core_logging
                      LABELS small)
  endforeach()
endif()
