ExternalProject_Add(apriltag
    PREFIX ${farm_ng_EXT_PREFIX}
    GIT_REPOSITORY https://github.com/AprilRobotics/apriltag
    GIT_TAG v3.3.0
    GIT_SHALLOW ON
    CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
    TEST_BEFORE_INSTALL OFF
)
