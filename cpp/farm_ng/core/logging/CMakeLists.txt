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
add_library(farm_ng_core::farm_ng_core_logging ALIAS farm_ng_core_logging)


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

set(TEST_BASENAMES
    assert_within
    expected
    format
    logger
    logger_define_trace_variant
    logger_define_debug_variant
    logger_define_info_variant)

foreach(test_basename ${TEST_BASENAMES})
  set(test_target ${test_basename}_test_farm_ng_core_logging)
  set(test_src ${test_basename}_test.cpp)
  add_executable(${test_target} ${test_src})
  target_link_libraries(${test_target} farm_ng_core_logging Eigen3::Eigen GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target})
  set_property(TEST ${test_target} PROPERTY LABELS small)
endforeach()
