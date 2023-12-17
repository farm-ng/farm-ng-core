#[[
farm_ng_core_logging

Console logging, string format and CHECK macros.

Depends on libfmt, expected (both part of future c++ standards),
thirdparty/farm_pp, and farm_ng_core_enum.

]]
add_library(farm_ng_core_logging SHARED 
    assert_within.h
    backtrace.h
    backtrace.cpp
    expected.h
    expected.cpp
    format.h
    format.cpp
    logger.h
    logger.cpp
    trace_debug_log.h
)

file(RELATIVE_PATH include_path_abs ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

target_include_directories(farm_ng_core_logging PUBLIC
"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${include_path_abs}>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(farm_ng_core_logging PUBLIC
  farm_ng_core_enum
  farm_ng_core::farm_pp
  fmt::fmt
  pthread
)


# TODO: Install


# find_package(Eigen3 3.4.0 REQUIRED)

# foreach(test_basename
#     assert_within
#     expected
#     format
#     logger
#     logger_define_trace_variant
#     logger_define_debug_variant
#     logger_define_info_variant)
#   farm_ng_add_test(${test_basename}
#                     PARENT_LIBRARY farm_ng_core_logging
#                     LINK_LIBRARIES farm_ng_core_logging Eigen3::Eigen
#                     LABELS small)
# endforeach()
