# project(Sophus VERSION 2.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(farm_ng_cmake_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${farm_ng_cmake_DIR}/farm_ng_cmakeConfig.cmake)

find_package(Eigen3 3.4.0 REQUIRED)


farm_ng_module(sophus2)
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


add_library(sophus2 INTERFACE)
target_link_libraries(sophus2 INTERFACE
  sophus2_common
  sophus2_calculus
  sophus2_linalg
  sophus2_lie
  sophus2_interp
  sophus2_interp_spline
  sophus2_geometry
)
add_library(farm_ng_core::sophus2 ALIAS sophus2)
install(TARGETS sophus2
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  COMPONENT Libs
)
