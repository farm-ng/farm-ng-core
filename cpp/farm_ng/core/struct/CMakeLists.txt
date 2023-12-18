add_library(farm_ng_core_struct INTERFACE 
    base.h
    macro_lib.h
    struct.h
)
add_library(farm_ng_core::farm_ng_core_struct ALIAS farm_ng_core_struct)

file(RELATIVE_PATH include_path_abs ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

target_include_directories(farm_ng_core_struct INTERFACE
"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${include_path_abs}>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(farm_ng_core_struct INTERFACE
  farm_ng_core::farm_pp
  farm_ng_core::farm_ng_core_proto_defs
)

# TODO: Install

set(TEST_BASENAMES
    base
    macro_lib
    struct)

foreach(test_basename ${TEST_BASENAMES})
  set(test_target ${test_basename}_test_farm_ng_core_struct)
  set(test_src ${test_basename}_test.cpp)
  add_executable(${test_target} ${test_src})
  target_link_libraries(${test_target} farm_ng_core_struct GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target})
  set_property(TEST ${test_target} PROPERTY LABELS small)
endforeach()
