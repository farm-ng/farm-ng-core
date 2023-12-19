#[[
sophus_color



]]

farm_ng_add_library(sophus_color
  NAMESPACE Sophus
  INCLUDE_DIR ../../
  HEADERS
  color.h
  SOURCES
  color.cpp
)
target_link_libraries(sophus_color PUBLIC sophus_calculus)

if(${BUILD_SOPHUS_TESTS})
  foreach(test_basename
    color)
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY sophus_color
                          LINK_LIBRARIES sophus_color
                          LABELS small)
  endforeach()
endif()
