#[[
core
]]

add_subdirectory(logging)
add_subdirectory(enum)
add_subdirectory(struct)
add_subdirectory(misc)
if(BUILD_FARM_NG_PROTOS)
  add_subdirectory(prototools)
  add_subdirectory(proto_conv)
  add_subdirectory(pipeline)
endif()
