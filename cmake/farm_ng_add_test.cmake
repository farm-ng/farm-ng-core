macro(farm_ng_enable_testing)
  # from https://google.github.io/googletest/quickstart-cmake.html
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)
endmacro(farm_ng_enable_testing)



macro(farm_ng_add_test target_basename)

  set(one_value_args PARENT_LIBRARY)
  set(multi_value_args
    LINK_LIBRARIES
    LABELS)
  cmake_parse_arguments(FARM_NG_ADD_TEST ""
                        "${one_value_args}" "${multi_value_args}" ${ARGN})

  # Appends ${FARM_NG_ADD_TEST_PARENT_LIBRARY} to make target name unique,
  # which is required by cmake.
  set(test_target ${target_basename}_test_${FARM_NG_ADD_TEST_PARENT_LIBRARY})
  set(test_cpp ${target_basename}_test.cpp)

  add_executable(${test_target} ${test_cpp})

  # Create custom target for building a group of tests that share a common label
  foreach(label ${FARM_NG_ADD_TEST_LABELS})
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

  target_link_libraries(${test_target} ${FARM_NG_ADD_TEST_PARENT_LIBRARY}
                        ${FARM_NG_ADD_TEST_LINK_LIBRARIES}
                        GTest::gtest_main)
  add_test(NAME ${test_target} COMMAND ${test_target} )
  set_property(TEST ${test_target} PROPERTY LABELS ${FARM_NG_ADD_TEST_LABELS})
endmacro()

