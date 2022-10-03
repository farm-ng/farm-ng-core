cmake_minimum_required(VERSION 3.16)
# https://semver.org/
project(farm_ng_core_super VERSION 0.0.1)

include(FetchContent)
FetchContent_Declare(
  farm_ng_cmake
  GIT_REPOSITORY https://github.com/farm-ng/farm-ng-cmake.git
  GIT_TAG 91e279de02ef3ca65c2dab17bf772ea31eb91de2
)

FetchContent_MakeAvailable(farm_ng_cmake)
set(farm_ng_cmake_DIR ${farm_ng_cmake_SOURCE_DIR}/cmake)
find_package(farm_ng_cmake REQUIRED)

include(External_fmt)
include(External_eigen)
include(External_expected)
#include(External_subdir_farm-ng-core)
ExternalProject_Add(farm-ng-core
    DEPENDS fmt expected eigen
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/farm-ng-core-build
    CMAKE_ARGS
        ${farm_ng_DEFAULT_ARGS}
        -DFARM_NG_BUILD_PROTOS=ON
        -DFARM_NG_BUILD_TESTS=ON
        -Dfarm_ng_cmake_DIR=${farm_ng_cmake_DIR}
    TEST_BEFORE_INSTALL ON
)


ExternalProject_Add(farm-ng-core_TestInstallTargets
    DEPENDS farm-ng-core
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../cpp/examples/test_install_targets
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/farm-ng-core_TestInstallTargets-build
    CMAKE_ARGS ${farm_ng_DEFAULT_ARGS}
    -DFARM_NG_BUILD_PROTOS=On
    INSTALL_COMMAND ""
)
