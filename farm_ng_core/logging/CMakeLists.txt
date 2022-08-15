#[[
core_logging

Console logging, string format and CHECK macros.

]]

set(base_logging_src_prefixes
  expected
  format
  logger)

set(base_logging_cpp)
set(base_logging_headers)

foreach(x ${base_logging_src_prefixes} )
    list(APPEND base_logging_cpp ${x}.cpp)
    list(APPEND base_logging_headers ${x}.h)
endforeach()

farm_ng_add_library(farm_ng_core_logging
  BUILD_INCLUDE_PATH
    ~/code/farm-ng-core/
  SOURCES
    ${base_logging_cpp}
  HEADERS
    ${base_logging_headers}
  LINK_LIBRARIES
    pthread
    boost_preprocessor
    expected
    fmt::fmt
    tbb
)

foreach(test_basename ${base_logging_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY farm_ng_core_logging
                   LINK_LIBRARIES 
                   LABELS small)
endforeach()
