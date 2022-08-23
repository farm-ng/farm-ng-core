#[[
core_logging

Console logging, string format and CHECK macros.

]]

set(base_logging_src_prefixes
  expected
  format
  logger)

add_library(fng_core_logging INTERFACE
  expected.h
  format.h
  logger.h)

target_link_libraries(fng_core_logging INTERFACE
  Boost::system
  fmt::fmt
  pthread
)

target_include_directories(fng_core_logging INTERFACE
  ../..
)

if (FARM_NG_CORE_TESTS)
  foreach(test_basename ${base_logging_src_prefixes})
  fng_core_add_test(${test_basename}
                    PARENT_LIBRARY fng_core_logging
                    LINK_LIBRARIES
                    LABELS small)
  endforeach()
endif()
