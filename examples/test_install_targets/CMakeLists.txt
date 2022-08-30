cmake_minimum_required(VERSION 3.24)
project(SophusInstallTargetTests)

set(CMAKE_CXX_STANDARD 17)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(fng_core 22.99.0 REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS headers)

set(example_source compile_all)

foreach(example_src ${example_source})
  add_executable( ${example_src} ${example_src}.cpp)
  target_link_libraries(${example_src} PUBLIC fmt::fmt)
endforeach()
