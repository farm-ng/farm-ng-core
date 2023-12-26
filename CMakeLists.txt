cmake_minimum_required(VERSION 3.24)

project(farm_ng_core VERSION 0.2.0)

# Custom farm-ng macros
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/farm_ng_cmakeConfig.cmake)

# Support local development package override
include(farm_ng_dev_packages)
# Support ccache
include(farm_ng_ccache)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED) 
find_package(Eigen3 3.4.0 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CLI11 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(cpp)
add_subdirectory(protos)

# add_executable(HelloWorld helloworld.cpp)
# target_link_libraries(HelloWorld PRIVATE farm_ng_core::farm_ng_core_logging)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# configure_file(
#   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/navidson.pc.in
#   ${CMAKE_CURRENT_BINARY_DIR}/navidson.pc
#   @ONLY
# )

# install(
#   FILES ${CMAKE_CURRENT_BINARY_DIR}/navidson.pc
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
# )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
)

# install(TARGETS house
#   EXPORT farm_ng_coreTargets
#   FILE_SET HEADERS
# )

install(EXPORT ${PROJECT_NAME}Targets NAMESPACE farm_ng:: DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})