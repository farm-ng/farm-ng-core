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


add_library(fng_core_utils INTERFACE
  erase.h
  filesystem.h
  stopwatch.h
  tokenize.h
  variant_utils.h
  void.h)

target_link_libraries(fng_core_utils INTERFACE
  fng_core_enum fng_core_logging
)
target_include_directories(fng_core_utils INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_sources(fng_core_utils INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../..
               FILES
                    erase.h
                    filesystem.h
                    stopwatch.h
                    tokenize.h
                    variant_utils.h
                    void.h)


if (FARM_NG_CORE_TESTS)
  foreach(test_basename ${base_enum_src_prefixes})
  fng_core_add_test(${test_basename}
                    PARENT_LIBRARY fng_core_utils
                    LABELS small)
  endforeach()
endif()
