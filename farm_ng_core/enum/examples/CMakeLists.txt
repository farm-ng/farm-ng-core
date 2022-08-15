find_package(CLI11 CONFIG REQUIRED)

add_executable(CliExample CliExample.cpp ${FNG_ENUM_HEADER_FILES})
target_link_libraries(CliExample PRIVATE CLI11::CLI11 ${STRING_VIEW_LIB} MyEnum)
target_include_directories(CliExample PRIVATE ${Boost_INCLUDE_DIRS})
