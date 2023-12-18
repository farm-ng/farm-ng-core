add_library(farm_ng_core_misc SHARED 
  conversions.h
  erase.h
  filesystem.h
  stopwatch.h
  shared.h
  concept_utils.h
  time_series.h
  tokenize.h
  variant_utils.h
  uri.h
  void.h
  time_series.cpp
)
add_library(farm_ng_core::farm_ng_core_misc ALIAS farm_ng_core_misc)

file(RELATIVE_PATH include_path_abs ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
target_include_directories(farm_ng_core_misc PUBLIC
"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${include_path_abs}>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(farm_ng_core_misc PUBLIC
  farm_ng_core_enum
  farm_ng_core_logging
  # Sophus::sophus_linalg
)

set(TEST_BASENAMES
    conversions
    erase
    filesystem
    shared
    stopwatch
    time_series
    tokenize
    uri
)

foreach(test_basename ${TEST_BASENAMES})
  set(test_target ${test_basename}_test_farm_ng_core_misc)
  set(test_src ${test_basename}_test.cpp)
  add_executable(${test_target} ${test_src})
  target_link_libraries(${test_target} farm_ng_core_misc GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target})
  set_property(TEST ${test_target} PROPERTY LABELS small)
endforeach()

# TODO
# add_subdirectory(proto)