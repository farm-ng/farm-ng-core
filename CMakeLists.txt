cmake_minimum_required(VERSION 3.24)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "cmake/farm_ng_provider.cmake")

# https://semver.org/
project(farm_ng_core VERSION 0.2.0)

# set(farm_ng_cmake_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# include(${farm_ng_cmake_DIR}/farm_ng_cmakeConfig.cmake)
# farm_ng_module(farm_ng_core)

# message("CMAKE_CXX_FLAGS" ${CMAKE_CXX_FLAGS})

# farm_ng_enable_testing()
enable_testing()


# set(CMAKE_CXX_CLANG_TIDY "")

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

# add_subdirectory(protos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(cpp)
