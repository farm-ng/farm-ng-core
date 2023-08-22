ExternalProject_Add(expected
    GIT_REPOSITORY  "https://github.com/TartanLlama/expected.git"
    GIT_TAG "b74fecd4448a1a5549402d17ddc51e39faa5020c"
    GIT_SHALLOW OFF # Can't use this because we're using a SHA and not a branch
    PREFIX ${farm_ng_EXT_PREFIX}
    CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
    -DEXPECTED_BUILD_TESTS=off
)
