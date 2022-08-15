#[[
core_utils

Standard library additions such as erase, string utils, file system,
threading tools etc.

Note: no geometry, no computer vision, not math (apart from simple unit
conversion etc.)
]]

set(base_utils_src_prefixes book_keeper
                            cli_enum
                            conversions
                            copy_on_write
                            erase
                            filesystem
                            stopwatch
                            thread_pool
                            time
                            to_from_proto
                            tokenize
                            uri)

set(base_utils_cpp)
set(base_utils_headers)

foreach(x ${base_utils_src_prefixes} )
    list(APPEND base_utils_cpp ${x}.cpp)
    list(APPEND base_utils_headers ${x}.h)
endforeach()

farm_ng_add_library(core_utils
  BUILD_INCLUDE_PATH
    /workspace/
  SOURCES
    ${base_utils_cpp}
  HEADERS
    ${base_utils_headers}
  LINK_LIBRARIES
    core_logging
    #core_utils_protobuf
    PRIVATE_LINK_LIBRARIES
    PUBLIC_INCLUDE_DIRS
)

foreach(test_basename ${base_utils_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY core_utils
                   LABELS small)
endforeach()
