#[[
sophus2_linalg



]]

set(sophus2_linalg_src_prefixes
    batch
    cast
    homogeneous
    orthogonal
    reduce
    vector_space
    vector_space_traits
)

set(sophus2_linalg_h)

foreach(x ${sophus2_linalg_src_prefixes} )
    list(APPEND sophus2_linalg_h ${x}.h)
endforeach()

farm_ng_add_library(sophus2_linalg
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  ${sophus2_linalg_h}
)

target_link_libraries(sophus2_linalg INTERFACE sophus2_common sophus_concept)

foreach(test_basename ${sophus2_linalg_src_prefixes})
  farm_ng_add_test(${test_basename}
                        PARENT_LIBRARY sophus2_linalg
                        LINK_LIBRARIES sophus2_linalg
                        LABELS small)
endforeach()
