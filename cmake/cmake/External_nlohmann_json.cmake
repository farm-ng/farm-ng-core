ExternalProject_Add(nlohmann_json
    GIT_REPOSITORY  "https://github.com/nlohmann/json.git"
    GIT_TAG "b2306145e1789368e6f261680e8dc007e91cc986"
    GIT_SHALLOW OFF # Can't use this because we're using a SHA and not a branch
    PREFIX ${farm_ng_EXT_PREFIX}
    CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
)
