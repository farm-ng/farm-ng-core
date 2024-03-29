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
  conversions.h
  erase.h
  filesystem.h
  stopwatch.h
  shared.h
  concept_utils.h
  time_series.h
  tokenize.h
  variant_utils.h
  uri.h
  void.h
  SOURCES
  time_series.cpp
)

target_link_libraries(farm_ng_core_misc PUBLIC
  farm_ng_core_enum farm_ng_core_logging
  Sophus::sophus_linalg
)

foreach(test_basename
    conversions
    erase
    filesystem
    shared
    stopwatch
    time_series
    tokenize
    uri
    )
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_misc
                    LINK_LIBRARIES farm_ng_core_misc
                    LABELS small)
endforeach()

if(BUILD_FARM_NG_PROTOS)
    add_subdirectory(proto)
endif()
