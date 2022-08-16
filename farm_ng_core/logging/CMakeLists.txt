#[[
core_logging

Console logging, string format and CHECK macros.

]]

set(base_logging_src_prefixes
  expected
  format
  logger)

add_library(farmngcore_logging INTERFACE
  expected.h
  format.h
  logger.h)

target_link_libraries(farmngcore_logging INTERFACE
  Boost::system
  expected
  fmt::fmt-header-only
  pthread
)

target_include_directories(farmngcore_logging INTERFACE
  ../..
)

if (FARM_NG_CORE_TESTS)
  foreach(test_basename ${base_logging_src_prefixes})
  farm_ng_core_add_test(${test_basename}
                        PARENT_LIBRARY farmngcore_logging
                        LINK_LIBRARIES
                        LABELS small)
  endforeach()
endif()
