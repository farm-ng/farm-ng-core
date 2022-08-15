

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


# farm_ng_add_library(core_enum
# BUILD_INCLUDE_PATH
# /workspace/
# SOURCES
# HEADERS
# ${base_enum_headers}
# LINK_LIBRARIES
# core_logging
# #core_utils_protobuf
# PRIVATE_LINK_LIBRARIES
# PUBLIC_INCLUDE_DIRS
# )



# set(FNG_ENUM_HEADER_IMPL_FILES
#     my_enum/impl/EnumDetails.h
#     my_enum/impl/EnumFlagsDetails.h
#     my_enum/impl/CompilerAttributes.h
#     my_enum/impl/StringView.h)

# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#     include(CTest)
# endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

# option(BUILD_EXAMPLES "Build examples." ON)
# if(BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()


# Following http://dominikberner.ch/cmake-interface-lib/

# target_include_directories(
#   ${PROJECT_NAME}
#   INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/>
#             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
# target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)