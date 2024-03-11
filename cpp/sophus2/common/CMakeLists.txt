#[[
sophus2_common



]]
farm_ng_add_library(sophus2_common
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  common.h
  enum.h
)

farm_ng_add_test(common
  PARENT_LIBRARY sophus2_common
  LINK_LIBRARIES sophus2_common
  LABELS small)

target_link_libraries(
  sophus2_common
  INTERFACE
    Eigen3::Eigen
    farm_ng_core::farm_ng_core_logging
  )
