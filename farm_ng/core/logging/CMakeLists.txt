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

list(APPEND base_logging_headers eigen.h)

farm_ng_add_library(core_logging
  BUILD_INCLUDE_PATH
    /workspace/
  SOURCES
    ${base_logging_cpp}
  HEADERS
    ${base_logging_headers}
  LINK_LIBRARIES
    pthread
    boost_preprocessor
    fmt::fmt
    tbb
  PUBLIC_INCLUDE_DIRS
    ${Eigen3_INCLUDE_DIRS}
)

foreach(test_basename ${base_logging_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY core_logging
                   LINK_LIBRARIES 
                   LABELS small)
endforeach()
