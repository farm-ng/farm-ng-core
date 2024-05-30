#[[
sophus2_image



]]

set(sophus2_image_src_prefixes
  interpolation
  image
  image_size
  layout
  image_types
  image_view
  mut_image
  mut_image_view
  mut_dyn_image
  mut_dyn_image_view
  dyn_image
  dyn_image_view
  dyn_image_types
  pixel_format
)

set(sophus2_image_cpp)
set(sophus2_image_h)
foreach(x ${sophus2_image_src_prefixes} )
    list(APPEND sophus2_image_cpp ${x}.cpp)
    list(APPEND sophus2_image_h ${x}.h)
endforeach()

farm_ng_add_library(sophus2_image
  NAMESPACE ${PROJECT_NAME}
  INCLUDE_DIR ../../
  HEADERS
  ${sophus2_image_h}
  SOURCES
  ${sophus2_image_cpp}
)
target_link_libraries(sophus2_image sophus2_geometry)



foreach(test_basename ${sophus2_image_src_prefixes})
  farm_ng_add_test(${test_basename}
                        PARENT_LIBRARY sophus2_image
                        LINK_LIBRARIES sophus2_image
                        LABELS small)
endforeach()

