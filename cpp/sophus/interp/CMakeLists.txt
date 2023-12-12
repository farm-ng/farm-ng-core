#[[
sophus_interp

]]

farm_ng_add_library(sophus_interp
  NAMESPACE Sophus
  INCLUDE_DIR ../../../
  HEADERS
  average.h
  interpolate.h
)
target_link_libraries(sophus_interp INTERFACE sophus_lie)

farm_ng_add_test(interpolate
                      PARENT_LIBRARY sophus_interp
                      LINK_LIBRARIES sophus_interp
                      LABELS small)

add_subdirectory(spline)
