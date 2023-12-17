include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)

# set(FARM_NG_PROVIDER_INSTALL_DIR ${CMAKE_SOURCE_DIR}/cppenv
#   CACHE PATH "The directory the farm-ng provider installs packages to"
# )
# Tell the built-in implementation to look in our area first, unless
# the find_package() call uses NO_..._PATH options to exclude it
# list(APPEND CMAKE_MODULE_PATH ${FARM_NG_PROVIDER_INSTALL_DIR}/cmake)
# list(APPEND CMAKE_PREFIX_PATH ${FARM_NG_PROVIDER_INSTALL_DIR})

macro(farm_ng_provide_dependency method package_name)

if("${package_name}" MATCHES "^(GTest|gtest|googletest)$")
    # message("Intercepted find_package(googletest)")
    # FetchContent_MakeAvailable(googletest)
    # set(${package_name}_FOUND TRUE)

# execute_process(
#     COMMAND some_tool ${package_name} --installdir ${FARM_NG_PROVIDER_INSTALL_DIR}
#     COMMAND_ERROR_IS_FATAL ANY
# )
endif()
endmacro()

cmake_language(SET_DEPENDENCY_PROVIDER farm_ng_provide_dependency SUPPORTED_METHODS FIND_PACKAGE)
