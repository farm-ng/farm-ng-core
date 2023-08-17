ExternalProject_Add(cmake
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake-build
    CMAKE_ARGS
    ${farm_ng_PREFIX_ARGS}
    INSTALL_COMMAND ""
)
