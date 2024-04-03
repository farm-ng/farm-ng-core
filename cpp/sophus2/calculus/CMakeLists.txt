#[[
sophus2_calculus



]]

farm_ng_add_library(sophus2_calculus
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  num_diff.h
  region.h
)
target_link_libraries(sophus2_calculus INTERFACE sophus2_common)

  farm_ng_add_test(region
    PARENT_LIBRARY sophus2_calculus
    LABELS small)
