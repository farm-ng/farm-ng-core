cmake_minimum_required(VERSION 3.16)
# https://semver.org/
project(farm_ng_all VERSION 0.0.1)

include(FetchContent)
FetchContent_Declare(
  farm_ng_cmake
  GIT_REPOSITORY https://github.com/farm-ng/farm-ng-cmake.git
  GIT_TAG main
)
FetchContent_MakeAvailable(farm_ng_cmake)
set(farm_ng_cmake_DIR ${farm_ng_cmake_SOURCE_DIR}/cmake)
find_package(farm_ng_cmake REQUIRED)

include(External_fmt)
include(External_eigen)
include(External_expected)
include(External_subdir_farm-ng-core)

ExternalProject_Add(farm-ng-core_TestInstallTargets
    DEPENDS farm-ng-core
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/farm-ng-core/cpp/examples/test_install_targets
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/farm-ng-core_TestInstallTargets-build
    CMAKE_ARGS ${farm_ng_DEFAULT_ARGS}
    -DBUILD_FARM_NG_PROTOS=${SUPER_PROJ_FARM_NG_PROTOS}
    INSTALL_COMMAND ""
)
