#[[
sophus2_lie



]]

set(sophus2_lie_src_prefixes
   group_manifold
   identity
   isometry2
   isometry3
   lie_group
   pose3
   rotation2
   rotation3
   scaling_translation
   scaling
   similarity2
   similarity3
   spiral_similarity2
   spiral_similarity3
   translation
)

set(sophus2_lie_h)

foreach(x ${sophus2_lie_src_prefixes} )
    list(APPEND sophus2_lie_h ${x}.h)
endforeach()



farm_ng_add_library(sophus2_lie
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  ${sophus2_lie_h}
)
target_link_libraries(sophus2_lie INTERFACE sophus2_linalg)


foreach(test_basename ${sophus2_lie_src_prefixes})
    farm_ng_add_test(${test_basename}
                          PARENT_LIBRARY sophus2_lie
                          LINK_LIBRARIES sophus2_lie
                          LABELS small)
endforeach()
