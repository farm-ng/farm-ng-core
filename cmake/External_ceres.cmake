ExternalProject_Add(ceres
    DEPENDS eigen
    GIT_REPOSITORY  https://ceres-solver.googlesource.com/ceres-solver
    GIT_TAG "2.1.0"
    GIT_SHALLOW ON
    PREFIX ${farm_ng_EXT_PREFIX}
    CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_TESTING=OFF
    -DBUILD_EXAMPLES=OFF
    -DBUILD_SHARED_LIBS=ON
    -DGFLAGS=OFF
    -DMINIGLOG=OFF
)
