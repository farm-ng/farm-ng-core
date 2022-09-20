#[[
farm_ng_core_logging

Console logging, string format and CHECK macros.

No dependency, but libfmt, expected (both part of future c++ standards) and
thirdparty/farm_pp.

]]

set(base_logging_src_prefixes
  expected
  format
  logger)

add_library(farm_ng_core_logging INTERFACE
  expected.h
  format.h
  logger.h)

target_link_libraries(farm_ng_core_logging INTERFACE
  farm_pp::farm_pp
  fmt::fmt
  pthread
)
target_include_directories(farm_ng_core_logging INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_sources(farm_ng_core_logging INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../../..
               FILES
                 eigen.h
                 expected.h
                 format.h
                 logger.h)



if (${FARM_NG_CORE_TESTS})
  foreach(test_basename ${base_logging_src_prefixes})
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_logging
                    LINK_LIBRARIES
                    LABELS small)
  endforeach()
endif()
