#[[
sophus2_sensor



]]

set(sophus2_sensor_src_prefixes
    camera_model
    clipping_planes
    imu_model
    camera_rig
    orthographic
    )

set(sophus2_sensor_cpp)
set(sophus2_sensor_h)
foreach(x ${sophus2_sensor_src_prefixes} )
    list(APPEND sophus2_sensor_cpp ${x}.cpp)
    list(APPEND sophus2_sensor_h ${x}.h)
endforeach()

farm_ng_add_library(sophus2_sensor SHARED
NAMESPACE ${PROJECT_NAME}
INCLUDE_DIR ../../
HEADERS
${sophus2_sensor_h}
camera_distortion/affine.h
camera_distortion/brown_conrady.h
camera_distortion/kannala_brandt.h
camera_projection/projection_z1.h
camera_projection/projection_ortho.h
SOURCES
  ${sophus2_sensor_cpp})

target_link_libraries(sophus2_sensor sophus2_lie sophus2_image)



foreach(test_basename ${sophus2_sensor_src_prefixes})
  farm_ng_add_test(${test_basename}
                        PARENT_LIBRARY sophus2_sensor
                        LINK_LIBRARIES sophus2_sensor
                        LABELS small)
endforeach()

