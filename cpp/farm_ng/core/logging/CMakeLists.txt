#[[
farm_ng_core_logging

Console logging, string format and CHECK macros.

No dependency, but libfmt, expected (both part of future c++ standards) and
thirdparty/farm_pp.

]]
farm_ng_add_library(farm_ng_core_logging
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    eigen.h
    expected.h
    format.h
    logger.h
    backtrace.h
  SOURCES
    format.cpp
    expected.cpp
    backtrace.cpp
)

target_link_libraries(farm_ng_core_logging PUBLIC
  farm_ng_core::farm_pp
  fmt::fmt
  pthread
)

foreach(test_basename
    expected
    format
    logger)
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_logging
                    LINK_LIBRARIES farm_ng_core_logging
                    LABELS small)
endforeach()
