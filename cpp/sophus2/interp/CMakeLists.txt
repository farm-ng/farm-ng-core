#[[
sophus2_interp

]]

farm_ng_add_library(sophus2_interp
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  average.h
  interpolate.h
)
target_link_libraries(sophus2_interp INTERFACE sophus2_lie)

farm_ng_add_test(interpolate
                      PARENT_LIBRARY sophus2_interp
                      LINK_LIBRARIES sophus2_interp
                      LABELS small)

add_subdirectory(spline)
