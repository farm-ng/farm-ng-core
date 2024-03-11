#[[
sophus2_color



]]

farm_ng_add_library(sophus2_color
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  color.h
  SOURCES
  color.cpp
)
target_link_libraries(sophus2_color PUBLIC sophus2_calculus)

if(${BUILD_SOPHUS_TESTS})
  foreach(test_basename
    color)
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY sophus2_color
                          LINK_LIBRARIES sophus2_color
                          LABELS small)
  endforeach()
endif()
