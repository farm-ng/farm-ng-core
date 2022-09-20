#[[
core
]]

add_subdirectory(logging)
add_subdirectory(enum)
add_subdirectory(misc)
if(Protobuf_FOUND)
  add_subdirectory(protobuf)
endif()
