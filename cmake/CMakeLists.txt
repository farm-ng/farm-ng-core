cmake_minimum_required(VERSION 3.16)
# don't update VERSION here, its in the farm_ng_cmakeConfigVersion.cmake file
project(farm_ng_core/cmake)

# NOTE assumes that cmake is a flat directory
file(GLOB FARM_NG_CMAKE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*")

install(FILES
    ${FARM_NG_CMAKE_FILES}
    DESTINATION
        share/farm_ng_core/cmake/cmake
    )


option(FARM_NG_DEV_BUILD Off)
if(NOT FARM_NG_DEV_BUILD)
    message(STATUS "To build farm-ng's development libraries:\n"
    "cmake -DFARM_NG_DEV_BUILD=ON .")
    return()
endif()

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/cmake)
# here we build a c++ development environment
include(ExternalProject)
include(farm_ng_default_args)

include(External_fmt)
include(External_expected)
include(External_eigen)
include(External_ceres)
include(External_apriltag)
include(External_cli11)
include(External_boost)
include(External_grpc)
include(External_nlohmann_json)

