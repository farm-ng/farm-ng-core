cmake_minimum_required(VERSION 3.23)
project(farm_ng_core VERSION 22.99.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if (NOT (CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL ""))
  # Default to  RelWithDebInfo if no build type is specified.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

# No NDEBUG for RelWithDebInfo. Among others, this makes sure that essential Eigen
# runtime asserts are enabled.
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")


option(FARM_NG_CORE_TESTS "Build tests" ON)

if(${FARM_NG_CORE_TESTS})
    enable_testing()
    find_package(GTest REQUIRED)
endif()


option(COMPILE_TIME_FMT "Use compile-time fmt" OFF)
if(COMPILE_TIME_FMT)
    add_definitions(-DFARM_NG_COMPILE_TIME_FMT)
endif()

option(ADDRESS_SANITIZER "Use Address Sanitizer, incompatible with THREAD_SANITIZER" OFF)
option(THREAD_SANITIZER "Use Thread Sanitizer, incompatible with ADDRESS_SANITIZER" OFF)



if (ADDRESS_SANITIZER)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
elseif (THREAD_SANITIZER)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unknown-warning-option")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-function -Wno-maybe-uninitialized -Wno-deprecated-copy")
endif()


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/farm_ng_core_add_test.cmake)

set(CMAKE_CXX_CLANG_TIDY "")
set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "")

set(EXPECTED_BUILD_TESTS FALSE)

find_package(fmt REQUIRED)

option(BUILD_FARM_NG_PROTOS "Build protos" OFF)

if (BUILD_FARM_NG_PROTOS)
  find_package(Protobuf REQUIRED)
endif()

if(BUILD_FARM_NG_PROTOS)
  farm_ng_add_protobufs(farm_ng_core_proto_defs
    PROTO_FILES
    ./protos/farm_ng/core.proto)
endif()

add_subdirectory(cpp)
