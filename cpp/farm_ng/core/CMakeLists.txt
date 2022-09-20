#[[
core
]]

add_subdirectory(logging)
add_subdirectory(enum)
add_subdirectory(misc)
if(BUILD_FARM_NG_CORE_PROTOS)
  add_subdirectory(protobuf)
endif()
