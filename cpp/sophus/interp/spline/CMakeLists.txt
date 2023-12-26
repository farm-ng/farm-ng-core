#[[
sophus_interp_spline

]]

farm_ng_add_library(sophus_interp_spline
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../../
  HEADERS
  bspline.h
  group_bspline.h
  details/bspline_segment.h
  details/cubic_basis.h
  details/group_bspline_segment.h
)
target_link_libraries(sophus_interp_spline INTERFACE sophus_lie)


farm_ng_add_test(spline
                      PARENT_LIBRARY sophus_interp_spline
                      LINK_LIBRARIES sophus_interp_spline
                      LABELS small)
