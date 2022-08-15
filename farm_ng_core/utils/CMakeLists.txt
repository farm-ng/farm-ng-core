#[[
core_utils

Standard library additions such as erase, string utils, file system,
threading tools etc.

Note: no geometry, no computer vision, not math (apart from simple unit
conversion etc.)
]]

set(base_enum_src_prefixes  erase
                            filesystem
                            stopwatch
                            tokenize
                            uri)

set(base_enum_cpp)
set(base_enum_headers)

foreach(x ${base_enum_src_prefixes} )
    list(APPEND base_enum_cpp ${x}.cpp)
    list(APPEND base_enum_headers ${x}.h)
endforeach()

farm_ng_add_library(farm_ng_core_utils
  BUILD_INCLUDE_PATH
    /workspace/
  SOURCES
    ${base_enum_cpp}
  HEADERS
    ${base_enum_headers}
  LINK_LIBRARIES
    farm_ng_core_logging
    PRIVATE_LINK_LIBRARIES
    PUBLIC_INCLUDE_DIRS
)

foreach(test_basename ${base_enum_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY farm_ng_core_utils
                   LABELS small)
endforeach()
