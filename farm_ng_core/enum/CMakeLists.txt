

set(core_enum_src_prefixes 
enum
enum_flags
enum_without_ostream
enum_flags_without_ostream)

set(base_enum_headers)

foreach(x ${base_enum_src_prefixes} )
list(APPEND base_enum_headers ${x}.h)
endforeach()

add_library(farm_ng_core_enum INTERFACE)

target_include_directories(farm_ng_core_enum INTERFACE
  ../..
  ../../thirdparty/preprocessor/include/
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
