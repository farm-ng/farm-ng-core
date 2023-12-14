project(Sophus VERSION 2.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(farm_ng_cmake_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

message("farm_ng_cmake_DIR" ${farm_ng_cmake_DIR})
include(${farm_ng_cmake_DIR}/farm_ng_cmakeConfig.cmake)

find_package(Eigen3 3.4.0 REQUIRED)


farm_ng_module(sophus)
message("COMPILE_OPTIONS" ${COMPILE_OPTIONS})




add_subdirectory(common)
add_subdirectory(concepts)
add_subdirectory(linalg)
add_subdirectory(calculus)
add_subdirectory(color)
add_subdirectory(lie)
add_subdirectory(interp)
add_subdirectory(geometry)
add_subdirectory(image)
add_subdirectory(sensor)
add_subdirectory(manifold)
add_subdirectory(ceres)


add_library(sophus INTERFACE)
target_link_libraries(sophus INTERFACE
  sophus_common
  sophus_calculus
  sophus_linalg
  sophus_lie
  sophus_interp
  sophus_interp_spline
  sophus_geometry
)
add_library (Sophus::sophus ALIAS sophus)
