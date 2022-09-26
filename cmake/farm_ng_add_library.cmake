macro(farm_ng_add_library target)
  set(one_value_args NAMESPACE INCLUDE_DIR INCLUDES)
  set(multi_value_args SOURCES HEADERS)
  cmake_parse_arguments(FARM_NG_ARGS ""
                        "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT DEFINED FARM_NG_ARGS_NAMESPACE)
    message(FATAL_ERROR "\nPlease specify NAMESPACE in farm_ng_add_library(${target})\n")
  endif()

  if(NOT DEFINED FARM_NG_ARGS_INCLUDE_DIR)
    message(FATAL_ERROR "\nPlease specify INCLUDE_DIR in farm_ng_add_library(${target})\n")
  endif()


  if((NOT DEFINED FARM_NG_ARGS_SOURCES) AND (DEFINED FARM_NG_ARGS_HEADERS))
    set(INTERFACE_OR_PUBLIC INTERFACE)
    add_library(${target} INTERFACE)
  elseif(DEFINED FARM_NG_ARGS_SOURCES)
    add_library(${target} SHARED)
    set(INTERFACE_OR_PUBLIC PUBLIC)
  endif()

  if(DEFINED FARM_NG_ARGS_SOURCES)
    target_sources(${target} PRIVATE ${FARM_NG_ARGS_SOURCES})
  endif()
  if(DEFINED FARM_NG_ARGS_HEADERS)
    target_sources(${target} ${INTERFACE_OR_PUBLIC} ${FARM_NG_ARGS_HEADERS})
  endif()
  
  #set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${FARM_NG_ARGS_HEADERS}")
  
  add_library(${FARM_NG_ARGS_NAMESPACE}::${target} ALIAS ${target})
  
  file(RELATIVE_PATH abs_include ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FARM_NG_ARGS_INCLUDE_DIR})
  target_include_directories(${target} ${INTERFACE_OR_PUBLIC}
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${abs_include}>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
  
  foreach ( file ${FARM_NG_ARGS_HEADERS} )
    file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR}/${FARM_NG_ARGS_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    
    get_filename_component( dir ${rel} DIRECTORY )
    #message(FATAL_ERROR "${FARM_NG_ARGS_INCLUDE_DIR} ${file} ${rel} ${dir}")
    install( FILES ${file} DESTINATION include/${dir} )
  endforeach()
endmacro()
