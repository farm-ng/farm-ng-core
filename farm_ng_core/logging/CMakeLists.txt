#[[
core_logging

Console logging, string format and CHECK macros.

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
  boost_preprocessor
  expected 
  fmt::fmt-header-only 
  pthread
)

target_include_directories(farm_ng_core_logging INTERFACE
  ../..
)

if (FARM_NG_CORE_TESTS)
  foreach(test_basename ${base_logging_src_prefixes})
  farm_ng_core_add_test(${test_basename}
                        PARENT_LIBRARY farm_ng_core_logging
                        LINK_LIBRARIES 
                        LABELS small)
  endforeach()
endif()