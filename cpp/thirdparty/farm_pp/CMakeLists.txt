# Copyright 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt


file(GLOB_RECURSE HPPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
farm_ng_add_library(farm_pp
    NAMESPACE farm_pp 
    INCLUDE_DIR include
    HEADERS ${HPPS}
)