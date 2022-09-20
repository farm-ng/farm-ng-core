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

macro(farm_ng_add_protobufs target)
  set(multi_value_args PROTO_FILES DEPENDENCIES)
  cmake_parse_arguments(FARM_NG_ADD_PROTOBUFS "" "" "${multi_value_args}" ${ARGN})
  set(${target}_PROTOBUF_IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Path to this project's protobuf sources")
  foreach(_dep_target ${FARM_NG_ADD_PROTOBUFS_DEPENDENCIES})
    list(APPEND DEP_PROTO_INCLUDES  -I ${${_dep_target}_PROTOBUF_IMPORT_DIRS})
  endforeach()

  # Create build directories for all supported languages
  list(APPEND _proto_languages cpp)
  foreach(_proto_language ${_proto_languages})
    set("_proto_output_dir_${_proto_language}" ${CMAKE_CURRENT_BINARY_DIR}/${_proto_language})
    file(MAKE_DIRECTORY "${_proto_output_dir_${_proto_language}}")
  endforeach()

  # Extract the module name from the target
  set(_cpp_out_sources)
  set(_cpp_out_headers)
  foreach (_proto_path ${FARM_NG_ADD_PROTOBUFS_PROTO_FILES})
    SET(_full_proto_path ${CMAKE_CURRENT_SOURCE_DIR}/${_proto_path})
    get_filename_component(_file_we ${_proto_path} NAME_WE)
    get_filename_component(_file_dir ${_proto_path} DIRECTORY)


    # cpp
    set("_protoc_args_cpp"
      "--cpp_out=${_proto_output_dir_cpp}")
    SET(_cpp_out_src ${_proto_output_dir_cpp}/${_file_dir}/${_file_we}.pb.cc)
    SET(_cpp_out_hdr ${_proto_output_dir_cpp}/${_file_dir}/${_file_we}.pb.h)
    list(APPEND _cpp_out_all ${_cpp_out_src} ${_cpp_out_hdr})
    list(APPEND _cpp_out_sources ${_cpp_out_src})
    list(APPEND _cpp_out_headers ${_cpp_out_hdr})
    add_custom_command(
      OUTPUT ${_cpp_out_src} ${_cpp_out_hdr}
      COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
      ARGS ${_protoc_args_cpp} -I ${CMAKE_CURRENT_SOURCE_DIR} ${DEP_PROTO_INCLUDES} ${_full_proto_path}
      DEPENDS ${_full_proto_path} ${PROTOBUF_PROTOC_EXECUTABLE}
      COMMENT "Generating cpp protobuf code for ${_proto_path}"
      VERBATIM)
  endforeach()

  add_library(${target} SHARED ${_cpp_out_sources})
  target_link_libraries(${target} ${Protobuf_LIBRARIES} ${FARM_NG_ADD_PROTOBUFS_DEPENDENCIES})



  target_include_directories(${target} PUBLIC
      "$<BUILD_INTERFACE:${_proto_output_dir_cpp}>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  )

  target_sources(${target} INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/cpp
               FILES ${_cpp_out_headers})


set_target_properties(${target}
  PROPERTIES CXX_CLANG_TIDY "")

  # Create custom target for building all protobus
  if (NOT TARGET protobuf_all)
    add_custom_target(protobuf_all)
  endif()
  add_dependencies(protobuf_all ${target})
endmacro()
