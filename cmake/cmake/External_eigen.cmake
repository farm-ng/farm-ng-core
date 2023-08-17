ExternalProject_Add(eigen
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG "bc57b926a0214340c67d91501f4672a014ecf8fe"
    GIT_SHALLOW OFF
    PREFIX ${farm_ng_EXT_PREFIX}
    CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
    -DEIGEN_DEFAULT_TO_ROW_MAJOR=$ROW_MAJOR_DEFAULT
)
