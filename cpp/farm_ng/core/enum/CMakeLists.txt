add_library(farm_ng_core_enum INTERFACE
    enum_flags_without_ostream.h
    enum_flags.h
    enum_without_iostream.h
    enum.h
    impl/enum_details.h
    impl/enum_flag_details.h
)

target_link_libraries(farm_ng_core_enum INTERFACE farm_ng_core::farm_pp)

file(RELATIVE_PATH include_path_abs ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# TODO: Use Filesets?
target_include_directories(farm_ng_core_enum INTERFACE
"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${include_path_abs}>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")


set(TEST_BASENAMES
      enum
      enum_without_iostream
      enum_flags
      enum_flags_without_ostream)

foreach(test_basename ${TEST_BASENAMES})
  set(test_target ${test_basename}_test_farm_ng_core_enum)
  set(test_src ${test_basename}_test.cpp)
  add_executable(${test_target} ${test_src})
  target_link_libraries(${test_target} farm_ng_core_enum GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target})
  set_property(TEST ${test_target} PROPERTY LABELS small)
endforeach()
