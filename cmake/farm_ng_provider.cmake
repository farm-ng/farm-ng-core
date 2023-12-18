# set(FARM_NG_PROVIDER_INSTALL_DIR ${CMAKE_SOURCE_DIR}/cppenv
#   CACHE PATH "The directory the farm-ng provider installs packages to"
# )
# Tell the built-in implementation to look in our area first, unless
# the find_package() call uses NO_..._PATH options to exclude it
# list(APPEND CMAKE_MODULE_PATH ${FARM_NG_PROVIDER_INSTALL_DIR}/cmake)
# list(APPEND CMAKE_PREFIX_PATH ${FARM_NG_PROVIDER_INSTALL_DIR})

set(FARM_NG_PROVIDER_DEV_PACKAGES "" CACHE STRING "Packages to import as git submodule")

macro(farm_ng_provide_dependency method package_name)

  if(${package_name} IN_LIST FARM_NG_PROVIDER_DEV_PACKAGES)
    message("Intercepted find_package ${package_name}")

    ExternalProject_Add(${package_name}
      SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/${package_name}"
      # PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${package_name}
      # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    )

    # FetchContent_MakeAvailable(googletest)
    # set(${package_name}_FOUND TRUE)

# execute_process(
#     COMMAND some_tool ${package_name} --installdir ${FARM_NG_PROVIDER_INSTALL_DIR}
#     COMMAND_ERROR_IS_FATAL ANY
# )
  endif()
endmacro()

cmake_language(SET_DEPENDENCY_PROVIDER farm_ng_provide_dependency SUPPORTED_METHODS FIND_PACKAGE)
