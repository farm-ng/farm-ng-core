macro(fng_core_logging target_basename)

  set(one_value_args PARENT_LIBRARY)
  set(multi_value_args
    LINK_LIBRARIES
    LABELS)
  cmake_parse_arguments(FNG_ADD_TEST ""
                        "${one_value_args}" "${multi_value_args}" ${ARGN})

  # Appends ${FNG_ADD_TEST_PARENT_LIBRARY} to make target name unique,
  # which is required by cmake.
  set(test_target ${target_basename}_test_${FNG_ADD_TEST_PARENT_LIBRARY})
  set(test_cpp ${target_basename}_test.cpp)

  add_executable(${test_target} ${test_cpp})

  # Create custom target for building a group of tests that share a common label
  foreach(label ${FNG_ADD_TEST_LABELS})
    if (NOT TARGET test_group_${label})
      add_custom_target(test_group_${label})
    endif()
    add_dependencies(test_group_${label} ${test_target})
  endforeach(label)

  # Create custom target for building all tests
  if (NOT TARGET test_group_all)
    add_custom_target(test_group_all)
  endif()
  add_dependencies(test_group_all ${test_target})

  target_link_libraries(${test_target} ${FNG_ADD_TEST_PARENT_LIBRARY}
                        ${FNG_ADD_TEST_LINK_LIBRARIES}
                        GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target} )
  set_property(TEST ${test_target} PROPERTY LABELS ${FNG_ADD_TEST_LABELS})
endmacro()
