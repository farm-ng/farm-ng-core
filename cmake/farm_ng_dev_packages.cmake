set(FARM_NG_PROVIDER_DEV_PACKAGES "" CACHE STRING "Packages to use local development versions of")

include(FetchContent)

if(FARM_NG_PROVIDER_DEV_PACKAGES)
    message("Local development versions enabled")
elseif()
    message("No local development versions enabled")
endif()
foreach(package_name IN LISTS FARM_NG_PROVIDER_DEV_PACKAGES)
    message(" - Using local development version of ${package_name} at ${CMAKE_SOURCE_DIR}/external/${package_name}")
    FetchContent_Declare(
        ${package_name}
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/${package_name}"
        OVERRIDE_FIND_PACKAGE
      )
endforeach()
