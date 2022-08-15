macro(farm_ng_module name VERSION)
  # Configure CCache if available
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  endif(CCACHE_FOUND)
  set(FARM_NG_MODULE_NAME ${name})
  set(FARM_NG_PROJECT_NAME farm_ng_${name})
  set(farm_ng_VERSION ${VERSION})

  message(STATUS "${FARM_NG_MODULE_NAME} version: -- ${VERSION} --")
  string(REPLACE "." ";" VERSION_LIST ${VERSION})
  list(GET VERSION_LIST 0 farm_ng_VERSION_MAJOR)
  list(GET VERSION_LIST 1 farm_ng_VERSION_MINOR)
  list(GET VERSION_LIST 2 farm_ng_VERSION_PATCH)

  include(CMakeToolsHelpers OPTIONAL)

  set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
  set(BUILD_SHARED_LIBS ON)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-function -Wno-maybe-uninitialized")
  endif()

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endmacro()

macro(farm_ng_add_library target)
  set(one_value_args BUILD_INCLUDE_PATH)

  set(multi_value_args
    SOURCES
    HEADERS
    LINK_LIBRARIES
    PUBLIC_INCLUDE_DIRS
    PRIVATE_LINK_LIBRARIES)

  cmake_parse_arguments(FARM_NG_ADD_LIBRARY "" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT DEFINED FARM_NG_ADD_LIBRARY_BUILD_INCLUDE_PATH)
    set(FARM_NG_ADD_LIBRARY_BUILD_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../ )
  endif()


  add_library(${target} SHARED
    ${FARM_NG_ADD_LIBRARY_SOURCES}
    ${FARM_NG_ADD_LIBRARY_HEADERS}
    )
  add_library(farm_ng_core::${target} ALIAS ${target})

  target_include_directories(${target} PUBLIC INTERFACE
    $<BUILD_INTERFACE:${FARM_NG_ADD_LIBRARY_BUILD_INCLUDE_PATH}>
    $<INSTALL_INTERFACE:include>
    )

  target_include_directories(${target} PRIVATE
    ${FARM_NG_ADD_LIBRARY_BUILD_INCLUDE_PATH}
    )

  if(DEFINED FARM_NG_ADD_LIBRARY_PUBLIC_INCLUDE_DIRS)
    target_include_directories(${target} PUBLIC
      ${FARM_NG_ADD_LIBRARY_PUBLIC_INCLUDE_DIRS}
      )
  endif()

  set_property(TARGET ${target} PROPERTY VERSION ${farm_ng_VERSION})
  set_property(TARGET ${target} PROPERTY SOVERSION ${farm_ng_MAJOR_VERSION})
  set_property(TARGET ${target} PROPERTY
    INTERFACE_${target}_MAJOR_VERSION ${farm_ng_MAJOR_VERSION})
  set_property(TARGET ${target} APPEND PROPERTY
    COMPATIBLE_INTERFACE_STRING farm_ng_MAJOR_VERSION
    )

  target_link_libraries(
    ${target} PUBLIC
    ${FARM_NG_ADD_LIBRARY_LINK_LIBRARIES}
    )

  target_link_libraries(
    ${target} PRIVATE
    ${FARM_NG_ADD_LIBRARY_PRIVATE_LINK_LIBRARIES}
    )

  install(TARGETS ${target} EXPORT ${FARM_NG_PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    COMPONENT Libs
    )

  install(
    FILES ${FARM_NG_ADD_LIBRARY_HEADERS}
    DESTINATION include/farm_ng_core/${FARM_NG_MODULE_NAME}
    COMPONENT Devel
    )
endmacro()

# Add an executable <target> and install command for the source file <target>.cpp
# An install COMPONENT must be provided as an option.
macro(farm_ng_add_executable_with_options target component)
  add_executable(${target} ${target}.cpp)

  install(
    TARGETS ${target}
    EXPORT ${FARM_NG_PROJECT_NAME}Targets
    RUNTIME DESTINATION bin
    COMPONENT ${component}
    )
endmacro()

# Add an executable <target> and install command for the source file <target>.cpp
# The install COMPONENT is set to `Bins`.
macro(farm_ng_add_executable target)
  farm_ng_add_executable_with_options(${target} Bins)
endmacro()

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
                        ${GTEST_MAIN_LIBRARY} ${GTEST_LIBRARY})
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
  string(REGEX REPLACE "farm_ng_|_protobuf" "" _module ${target})
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

  farm_ng_add_library(${target} SOURCES ${_cpp_out_sources}
    HEADERS ${_cpp_out_headers}
    PUBLIC_INCLUDE_DIRS ${Protobuf_INCLUDE_DIRS}
    BUILD_INCLUDE_PATH ${_proto_output_dir_cpp}
    LINK_LIBRARIES  ${Protobuf_LIBRARIES} ${FARM_NG_ADD_PROTOBUFS_DEPENDENCIES})

  set_target_properties(${target}
      PROPERTIES CXX_CLANG_TIDY "")

  # Create custom target for building all protobus
  if (NOT TARGET protobuf_all)
    add_custom_target(protobuf_all)
  endif()
  add_dependencies(protobuf_all ${target})
endmacro()


macro(farm_ng_export_module)
set(module_name ${FARM_NG_PROJECT_NAME})

set(_EXPORT_DIR ${CMAKE_BINARY_DIR}/export/${module_name})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${_EXPORT_DIR}/${module_name}ConfigVersion.cmake"
  VERSION ${farm_ng_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${module_name}Targets
  FILE "${_EXPORT_DIR}/${module_name}Targets.cmake"
  NAMESPACE farm_ng_core::
)

configure_file(cmake/${module_name}Config.cmake
  "${_EXPORT_DIR}/${module_name}Config.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/${module_name})
install(EXPORT ${module_name}Targets
  FILE
  ${module_name}Targets.cmake
  NAMESPACE
    farm_ng_core::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/${module_name}Config.cmake
    "${_EXPORT_DIR}/${module_name}ConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

endmacro()
