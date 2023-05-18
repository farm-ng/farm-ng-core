set(farm_ng_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/prefix" CACHE STRING "Choose where to install things.")
set(farm_ng_EXT_PREFIX "${CMAKE_BINARY_DIR}/ext")
set(BUILD_SHARED_LIBS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build." FORCE)
  message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified.")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


set(farm_ng_PREFIX_ARGS
  "-DCMAKE_PREFIX_PATH:PATH=${farm_ng_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${farm_ng_INSTALL_PREFIX}"
)

set(farm_ng_DEFAULT_ARGS
  "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON"
  "-DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}"
  "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
  "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
  "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=True" # https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
  ${farm_ng_PREFIX_ARGS}
)

include(ExternalProject)
