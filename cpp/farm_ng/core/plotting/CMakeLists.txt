#[[
farm_ng_pipeline tools

General productivity tools for the farm-ng-core component pipeline.

]]


set(farm_ng_component_src_prefixes
    plotting_component
    types
    remote_plotting_client
)

set(farm_ng_component_cpp)
set(farm_ng_component_headers)
foreach(x ${farm_ng_component_src_prefixes} )
    list(APPEND farm_ng_component_cpp ${x}.cpp)
    list(APPEND farm_ng_component_headers ${x}.h)
endforeach()

farm_ng_add_library(farm_ng_core_plotting
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../..
  HEADERS
    ${farm_ng_component_headers}
  SOURCES
    ${farm_ng_component_cpp}
)

target_link_libraries(farm_ng_core_plotting PUBLIC
  farm_ng_core::farm_ng_core_logging
  farm_ng_core::farm_ng_core_proto_conv_linalg
  farm_ng_core::farm_ng_core_proto_conv_calculus
    farm_ng_core::farm_ng_core_proto_conv_plotting
  farm_ng_core::farm_ng_core_pipeline
  farm_ng_core::farm_ng_core_proto_defs
)

foreach(test_basename ${farm_ng_component_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY farm_ng_core_plotting
                   LABELS small)
endforeach()

add_executable(remote_plot_example remote_plot_example.cpp)
target_link_libraries(remote_plot_example
  farm_ng_core_plotting
  CLI11::CLI11
)
