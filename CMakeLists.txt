cmake_minimum_required(VERSION 3.24)

# Installs the farm-ng dependency provider, which
# provides a custom hook for find_package() that allows for 
# easy switching between vcpkg-installed dependencies and submodule dependencies
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "cmake/farm_ng_provider.cmake")

project(farm_ng_core VERSION 0.2.0)

# Custom farm-ng macros
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/farm_ng_cmakeConfig.cmake)

enable_testing()

find_package(fmt REQUIRED)
find_package(GTest REQUIRED) 
find_package(Eigen3 3.4.0 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CLI11 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(cpp)
add_subdirectory(protos)
