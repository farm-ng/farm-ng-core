
add_library(farm_ng_core_enum INTERFACE 
  enum.h 
  enum_flags.h 
  enum_without_ostream.h 
  enum_flags_without_ostream.h)

  target_include_directories(farm_ng_core_enum INTERFACE
  ../..
)

if (FARM_NG_CORE_TESTS)
  add_subdirectory(tests)
endif()