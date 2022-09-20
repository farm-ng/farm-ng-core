

add_subdirectory(thirdparty/farm_pp)
add_subdirectory(farm_ng/core)

set(install_library_targets
  farm_pp farm_ng_core_logging
  farm_ng_core_enum
  farm_ng_core_misc)

if(BUILD_FARM_NG_CORE_PROTOS)
  message(STATUS "Protobuf found, hence we will be building farm_ng_core_protobuf")
  list(APPEND install_library_targets farm_ng_core_protobuf farm_ng_core_misc_proto farm_ng_core_misc_proto_defs)
endif()

install(
  TARGETS
     ${install_library_targets}
  EXPORT farm_ng_coreTargets
  FILE_SET HEADERS
  LIBRARY DESTINATION lib
)

write_basic_package_version_file(
    farm_ng_coreConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

install(EXPORT farm_ng_coreTargets
    FILE farm_ng_coreTargets.cmake
    NAMESPACE farm_ng::
    DESTINATION share/farm_ng_core/cmake
)

configure_file(../cmake/farm_ng_coreConfig.cmake.in farm_ng_coreConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/farm_ng_coreConfigVersion.cmake"
        DESTINATION share/farm_ng_core/cmake)
