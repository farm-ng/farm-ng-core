cmake_minimum_required(VERSION 3.24)

project(farm_ng_core_downstream VERSION 0.2.0)

find_package(farm_ng_core REQUIRED)

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld PRIVATE farm_ng_core::farm_ng_core_logging)