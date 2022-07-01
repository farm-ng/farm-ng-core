# Copyright 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

file(GLOB_RECURSE HPPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.hpp")

add_library(farm_pp INTERFACE ${HPPS})

target_include_directories(farm_pp INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_sources(farm_pp INTERFACE
               FILE_SET HEADERS
               BASE_DIRS include/
               FILES ${HPPS})
