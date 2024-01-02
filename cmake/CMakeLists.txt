# Install CMake modules so they can reused downstream
# Reference:
# "Modern CMake Modules - Bret Brown - CppCon 2021"
#   https://www.youtube.com/watch?v=IZXNsim9TWI

install(
    FILES
        farm_ng_add_library.cmake
        farm_ng_add_protobufs.cmake
        farm_ng_add_test.cmake
        farm_ng_ccache.cmake
        farm_ng_dev_packages.cmake
        farm_ng_macos_ssl_fix.cmake
        farm_ng_module.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
    COMPONENT cmake-farm-ng-core
)