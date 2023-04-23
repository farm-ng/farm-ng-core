#[[
farm_ng_base_pipeline

Component framework to define threadsafe pipelines.

]]

set(CMAKE_CXX_STANDARD 17)

set(farm_ng_pipeline_src_prefixes
       component
                          context
                          input
                          output
                          thread_pool
)

set(farm_ng_pipeline_cpp)
set(farm_ng_pipeline_headers)
foreach(x ${farm_ng_pipeline_src_prefixes} )
    list(APPEND farm_ng_pipeline_cpp ${x}.cpp)
    list(APPEND farm_ng_pipeline_headers ${x}.h)
endforeach()

farm_ng_add_library(farm_ng_pipeline
  NAMESPACE farm_ng_core
  INCLUDE_DIR ../../..
  HEADERS
    ${farm_ng_pipeline_headers}
  SOURCES
    ${farm_ng_pipeline_cpp}
)

target_link_libraries(farm_ng_pipeline PUBLIC
  farm_ng_core::farm_ng_core_logging # Boost::coroutine
)

foreach(test_basename ${farm_ng_pipeline_src_prefixes})
  farm_ng_add_test(${test_basename}
                   PARENT_LIBRARY farm_ng_pipeline
                   LABELS small)
endforeach()
