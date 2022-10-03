#[[
core
]]

add_subdirectory(logging)
add_subdirectory(enum)
add_subdirectory(misc)
if(FARM_NG_BUILD_PROTOS)
  add_subdirectory(prototools)
endif()
