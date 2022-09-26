cmake_minimum_required(VERSION 3.16)
project(SophusInstallTargetTests)

set(CMAKE_CXX_STANDARD 17)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(farm_ng_core 0.0.1 REQUIRED)
find_package(fmt REQUIRED)

set(example_source )
find_package(Protobuf QUIET)

if (BUILD_FARM_NG_PROTOS)
  find_package(Protobuf REQUIRED)

  add_executable(compile_all_proto compile_all_proto.cpp)
  target_link_libraries(compile_all_proto PUBLIC 
    farm_ng_core::farm_ng_core_misc_proto_conv)
endif()

add_executable(compile_all compile_all.cpp)
target_link_libraries(compile_all PUBLIC 
  farm_ng_core::farm_ng_core_logging)
