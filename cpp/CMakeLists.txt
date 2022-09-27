add_subdirectory(thirdparty/farm_pp)

add_subdirectory(farm_ng/core)


if(FALSE)
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
# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#exporting-targets-from-the-build-tree
export(EXPORT farm_ng_coreTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreTargets.cmake"
    NAMESPACE farm_ng_core::
)


configure_file(../cmake/farm_ng_coreConfig.cmake.in farm_ng_coreConfig.cmake @ONLY)

foreach( cmake_file
    ../cmake/farm_ng_add_library.cmake
    ../cmake/farm_ng_add_protobufs.cmake
    ../cmake/farm_ng_add_test.cmake
    ../cmake/farm_ng_default_args.cmake)
  configure_file(${cmake_file} ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
endforeach()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfigVersion.cmake"
        DESTINATION share/farm_ng_core/cmake)
endif()
