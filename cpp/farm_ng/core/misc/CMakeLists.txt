#[[
farm_ng_core_misc

Header-only standard library additions such as erase, string utils, file system,
threading tools and other general purpose miscellaneous utilities.

Note: no geometry, no computer vision, not math.

Only depends on farm_ng_core_enum and farm_ng_core_logging and should not
pull in additional dependency in the future.

]]

set(base_enum_src_prefixes  erase
                            filesystem
                            stopwatch
                            tokenize
                            uri)


add_library(farm_ng_core_misc INTERFACE
  erase.h
  filesystem.h
  stopwatch.h
  tokenize.h
  variant_utils.h
  uri.h
  void.h)

target_link_libraries(farm_ng_core_misc INTERFACE
  farm_ng_core_enum farm_ng_core_logging
)
target_include_directories(farm_ng_core_misc INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../..>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_sources(farm_ng_core_misc INTERFACE
               FILE_SET HEADERS
               BASE_DIRS ../../..
               FILES
                    erase.h
                    filesystem.h
                    stopwatch.h
                    tokenize.h
                    variant_utils.h
                    uri.h
                    void.h)


if (${FARM_NG_CORE_TESTS})
  foreach(test_basename ${base_enum_src_prefixes})
  farm_ng_add_test(${test_basename}
                    PARENT_LIBRARY farm_ng_core_misc
                    LABELS small)
  endforeach()
endif()


if(BUILD_FARM_NG_PROTOS)
    add_subdirectory(proto)
endif()
