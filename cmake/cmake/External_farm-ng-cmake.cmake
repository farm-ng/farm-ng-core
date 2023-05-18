ExternalProject_Add(farm-ng-cmake
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/farm-ng-cmake
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/farm-ng-cmake-build
    CMAKE_ARGS
    ${farm_ng_PREFIX_ARGS}
    INSTALL_COMMAND ""
)
