
add_library(farm_ng_core_enum INTERFACE
  enum.h
  enum_flags.h
  enum_without_ostream.h
  enum_flags_without_ostream.h)

  target_include_directories(farm_ng_core_enum INTERFACE
  ../..
)

if (FARM_NG_CORE_TESTS)
  set(FNG_ENUM_TESTS
      enum
      enum_without_ostream
      enum_flags
      enum_flags_without_ostream)

  foreach(test_basename ${FNG_ENUM_TESTS})
    farm_ng_core_add_test(${test_basename}
                          PARENT_LIBRARY farm_ng_core_enum
                          LINK_LIBRARIES farm_ng_core_logging
                          LABELS small)
  endforeach()
endif()
