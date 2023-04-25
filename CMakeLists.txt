cmake_minimum_required(VERSION 3.16)
# https://semver.org/
project(farm_ng_core VERSION 0.2.0)

find_package(farm_ng_cmake REQUIRED)

farm_ng_module()
farm_ng_enable_testing()

set(CMAKE_CXX_CLANG_TIDY "")

find_package(fmt REQUIRED)

add_subdirectory(protos)
add_subdirectory(cpp)

farm_ng_export_module(
  NAME farm_ng_core
  REQUIRED_DEPS fmt)
