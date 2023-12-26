include(CMakeFindDependencyMacro)

find_dependency(fmt REQUIRED)
find_dependency(Eigen3 3.4.0 REQUIRED)
find_dependency(Protobuf REQUIRED)
find_dependency(gRPC REQUIRED)
find_dependency(Boost REQUIRED COMPONENTS system)

include(${CMAKE_CURRENT_LIST_DIR}/farm_ng_coreTargets.cmake)