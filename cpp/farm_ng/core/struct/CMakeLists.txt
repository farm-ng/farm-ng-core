#[[
farm_ng_core_struct

Set of macros for compile-time enum introspection / autogeneration of auxiliary
helper functions.

Depends on third_party/farm_pp.

]]

set(
  farm_ng_core_struct_files
      base
      macro_lib
      struct)

set(farm_ng_core_core_struct_headers)
foreach(x ${farm_ng_core_struct_files} )
    list(APPEND farm_ng_core_core_struct_headers ${x}.h)
endforeach()


farm_ng_add_library(farm_ng_core_struct
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
   base
   macro_lib
   struct
)

target_link_libraries(farm_ng_core_struct
  INTERFACE farm_ng_core::farm_ng_core_logging farm_ng_core::farm_ng_core_proto_defs)


foreach(test_basename ${farm_ng_core_struct_files})
    farm_ng_add_test(${test_basename}
                      PARENT_LIBRARY farm_ng_core_struct
                      LINK_LIBRARIES farm_ng_core_struct
                      LABELS small)
endforeach()
