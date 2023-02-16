#[[
farm_ng_core_logging

Console logging, string format and CHECK macros.

Depends on libfmt, expected (both part of future c++ standards),
thirdparty/farm_pp, and farm_ng_core_enum.

]]
farm_ng_add_library(farm_ng_core_logging
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    eigen.h
    expected.h
    format.h
    logger_impl.h
    backtrace.h
  SOURCES
    format.cpp
    expected.cpp
    logger_impl.cpp
    backtrace.cpp
)

target_link_libraries(farm_ng_core_logging PUBLIC
  farm_ng_core_enum
  farm_ng_core::farm_pp
  fmt::fmt
  pthread
)

foreach(test_basename
    expected
    format
    logger
    logger_define)
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_logging
                    LINK_LIBRARIES farm_ng_core_logging
                    LABELS small)
endforeach()
