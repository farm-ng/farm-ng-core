add_subdirectory(thirdparty/farm_pp)

add_subdirectory(farm_ng/core)

write_basic_package_version_file(
    farm_ng_coreConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

install(EXPORT farm_ng_coreTargets
    FILE farm_ng_coreTargets.cmake
    NAMESPACE farm_ng_core::
    DESTINATION share/farm_ng_core/cmake
)

configure_file(../cmake/farm_ng_coreConfig.cmake.in farm_ng_coreConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfigVersion.cmake"
        DESTINATION share/farm_ng_core/cmake)
