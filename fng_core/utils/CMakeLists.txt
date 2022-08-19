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
                            tokenize)


add_library(farmngcore_utils INTERFACE
  erase.h
  filesystem.h
  stopwatch.h
  tokenize.h
  variant_utils.h
  void.h)

target_link_libraries(farmngcore_utils INTERFACE
  farm_ng_core_enum farmngcore_logging
)

target_include_directories(farmngcore_utils INTERFACE
  ../..
)


if (FARM_NG_CORE_TESTS)
  foreach(test_basename ${base_enum_src_prefixes})
  farm_ng_core_add_test(${test_basename}
                        PARENT_LIBRARY farmngcore_utils
                        LABELS small)
  endforeach()
endif()
