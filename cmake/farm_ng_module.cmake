set(FARM_NG_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})
option(FARM_NG_INSTALL_MODULES "Install farm-ng modules, off by default which makes them in source for development" Off)

include(CMakePackageConfigHelpers)

macro(farm_ng_module)

  # message(STATUS "farm_ng_module ${PROJECT_NAME}")
  # # Configure CCache if available
  # find_program(CCACHE_FOUND ccache)
  # if(CCACHE_FOUND)
  #   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  #   set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  # endif(CCACHE_FOUND)


  # include(CMakeToolsHelpers OPTIONAL)
  # No NDEBUG for RelWithDebInfo. Among others, this makes sure that essential Eigen
  # runtime asserts are enabled.
  # set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

  # set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
  # set(BUILD_SHARED_LIBS ON)
  # set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # Coarsely, the c++ standard used is c++17 plus concepts from c++20.
  # if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  #   # For clang, c++20 is enabled, but only concepts shall be used from the features newly added.
  #   set(CMAKE_CXX_STANDARD 20)
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wsign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedef -Wreorder-ctor -Wreorder-init-list")
  # elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  #   set(CMAKE_CXX_STANDARD 17)
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-function -Wno-maybe-uninitialized")
  # endif()

  # if(BUILD_ASAN)
  #   if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  #     message(FATAL_ERROR "need to build with Debug when using asan: " ${CMAKE_BUILD_TYPE})
  #   endif()
  #   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  #       -fsanitize=address \
  #       -fsanitize=bool \
  #       -fsanitize=bounds \
  #       -fsanitize=enum \
  #       -fsanitize=float-cast-overflow \
  #       -fsanitize=float-divide-by-zero \
  #       -fsanitize=nonnull-attribute \
  #       -fsanitize=returns-nonnull-attribute \
  #       -fsanitize=signed-integer-overflow \
  #       -fsanitize=undefined \
  #       -fsanitize=vla-bound \
  #       -fno-sanitize=alignment \
  #       -fsanitize=leak \
  #   ")
  # endif()

  # set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endmacro()


macro(farm_ng_export_module)
  set(one_value_args NAME)
  set(multi_value_args REQUIRED_DEPS OPTIONAL_DEPS)
  cmake_parse_arguments(FARM_NG_EXPORT_MODULE ""
                        "${one_value_args}" "${multi_value_args}" ${ARGN})


  write_basic_package_version_file(
    ${FARM_NG_EXPORT_MODULE_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )


  add_library(${FARM_NG_EXPORT_MODULE_NAME}_module INTERFACE)

  # https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#exporting-targets-from-the-build-tree
  install(EXPORT  ${FARM_NG_EXPORT_MODULE_NAME}Targets
    FILE ${FARM_NG_EXPORT_MODULE_NAME}Targets.cmake
    NAMESPACE ${FARM_NG_EXPORT_MODULE_NAME}::
    DESTINATION share/${FARM_NG_EXPORT_MODULE_NAME}/cmake
    )

  export(EXPORT ${FARM_NG_EXPORT_MODULE_NAME}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${FARM_NG_EXPORT_MODULE_NAME}Targets.cmake"
    NAMESPACE ${FARM_NG_EXPORT_MODULE_NAME}::
    )

  configure_file(${FARM_NG_CMAKE_DIR}/farm_ng_moduleConfig.cmake.in ${FARM_NG_EXPORT_MODULE_NAME}Config.cmake @ONLY)
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${FARM_NG_EXPORT_MODULE_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${FARM_NG_EXPORT_MODULE_NAME}ConfigVersion.cmake"
    DESTINATION
    share/${FARM_NG_EXPORT_MODULE_NAME}/cmake)

  set(${FARM_NG_EXPORT_MODULE_NAME}_PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos CACHE STRING "" FORCE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE ${FARM_NG_EXPORT_MODULE_NAME})
endmacro()

macro(farm_ng_external_module NAME)
  set(one_value_args SOURCE_DIR BUILD_TYPE)
  set(multi_value_args DEPENDS MODULE_DEPENDS CMAKE_ARGS)
  cmake_parse_arguments(FARM_NG_ARGS ""
                        "${one_value_args}" "${multi_value_args}" ${ARGN})

  set(_BINARY_DIR                        ${CMAKE_BINARY_DIR}/${NAME}-build)
  set(dep_dir_flags)
  set(${NAME}_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build.")
  set_property(CACHE ${NAME}_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
  if(FARM_NG_ARGS_BUILD_TYPE)
    set(${NAME}_BUILD_TYPE "${FARM_NG_ARGS_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  endif()



  if(FARM_NG_INSTALL_MODULES)
    set(INSTALL_COMMAND_OPTION cmake --build . --target install)
  else()
    set(INSTALL_COMMAND_OPTION cmake -E echo "Skipping install step.")
    foreach(dep ${FARM_NG_ARGS_MODULE_DEPENDS})
        set(dep_dir_flags ${dep_dir_flags};-D${dep}_DIR=${CMAKE_BINARY_DIR}/${dep}-build)
    endforeach()
  endif()



    ExternalProject_Add(${NAME}
        DEPENDS ${FARM_NG_ARGS_DEPENDS} ${FARM_NG_ARGS_MODULE_DEPENDS}
        PREFIX ${farm_ng_EXT_PREFIX}
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${FARM_NG_ARGS_SOURCE_DIR}
        BINARY_DIR ${_BINARY_DIR}
        CMAKE_ARGS
        ${farm_ng_DEFAULT_ARGS}
        ${FARM_NG_ARGS_CMAKE_ARGS}
        -DBUILD_FARM_NG_PROTOS=ON
        -DCMAKE_BUILD_TYPE=${${NAME}_BUILD_TYPE}
        -Dfarm_ng_cmake_DIR=${farm_ng_cmake_DIR}
        ${dep_dir_flags}
        TEST_BEFORE_INSTALL OFF
        INSTALL_COMMAND "${INSTALL_COMMAND_OPTION}"
    )
endmacro()
