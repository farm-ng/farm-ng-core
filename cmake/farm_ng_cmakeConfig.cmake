list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR})

include(farm_ng_macos_ssl_fix)
include(ExternalProject)
include(farm_ng_module)
include(farm_ng_add_test)
include(farm_ng_add_library)
if(NOT FARM_NG_SUPER_PROJECT_BUILD_PROTOBUF)
  include(farm_ng_add_protobufs)
endif()
include(farm_ng_default_args)
