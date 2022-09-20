cmake_minimum_required(VERSION 3.23)
project(SophusInstallTargetTests)

set(CMAKE_CXX_STANDARD 17)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(farm_ng_core 22.99.0 REQUIRED)
find_package(fmt REQUIRED)

set(example_source compile_all)
find_package(Protobuf QUIET)

if(Protobuf_FOUND)
  list(APPEND example_source compile_all_proto)
endif()

foreach(example_src ${example_source})
  add_executable( ${example_src} ${example_src}.cpp)
  target_link_libraries(${example_src} PUBLIC fmt::fmt)
endforeach()
