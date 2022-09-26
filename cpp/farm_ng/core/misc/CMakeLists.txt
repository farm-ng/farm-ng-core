#[[
farm_ng_core_misc

Header-only standard library additions such as erase, string utils, file system,
threading tools and other general purpose miscellaneous utilities.

Note: no geometry, no computer vision, not math.

Only depends on farm_ng_core_enum and farm_ng_core_logging and should not
pull in additional dependency in the future.

]]

farm_ng_add_library(farm_ng_core_misc
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
  erase.h
  filesystem.h
  stopwatch.h
  tokenize.h
  variant_utils.h
  uri.h
  void.h
)

target_link_libraries(farm_ng_core_misc INTERFACE
  farm_ng_core_enum farm_ng_core_logging
)

if (${FARM_NG_CORE_TESTS})
  foreach(test_basename 
    erase
    filesystem
    stopwatch
    tokenize
    uri
    )
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_misc
                    LINK_LIBRARIES farm_ng_core_misc
                    LABELS small)
  endforeach()
endif()


if(BUILD_FARM_NG_PROTOS)
    add_subdirectory(proto)
endif()
