#[[
sophus2_geometry



]]

farm_ng_add_library(sophus2_geometry
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  fit_plane.h
  inverse_depth.h
  plane_conv.h
  point_transform.h
  ray.h
)
target_link_libraries(sophus2_geometry INTERFACE sophus2_lie)

foreach(test_basename
  fit_plane
  inverse_depth
  plane_conv
  point_transform
  ray)
  farm_ng_add_test(${test_basename}
                        PARENT_LIBRARY sophus2_geometry
                        LINK_LIBRARIES sophus2_geometry
                        LABELS small)
endforeach()
