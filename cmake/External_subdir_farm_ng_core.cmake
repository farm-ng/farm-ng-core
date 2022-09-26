ExternalProject_Add(farm-ng-core
    DEPENDS fmt expected
    PREFIX ${farm_ng_EXT_PREFIX}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/farm-ng-core
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/farm-ng-core-build
    CMAKE_ARGS ${farm_ng_DEFAULT_ARGS} -DBUILD_FARM_NG_PROTOS=On
    TEST_AFTER_INSTALL ON
)

