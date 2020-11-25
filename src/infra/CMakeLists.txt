# install headers 
file(GLOB_RECURSE HEADERS *.h *.hpp)
install(FILES ${HEADERS} DESTINATION include/src/infra)

set(_SUBDIRNAME infra)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})

file(GLOB SRCS ${CMAKE_CURRENT_LIST_DIR}/*.c*)

link_directories(${TCL_DIR}/lib)

add_library(${_SUBLIBNAME} STATIC ${SRCS})
target_include_directories(${_SUBLIBNAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    ${PROFILER_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${TCL_DIR}/include 
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR})
	
# must turn fPIC on 
set_target_properties(${_SUBLIBNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
install(TARGETS ${_SUBLIBNAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )

# install headers
file(GLOB_RECURSE HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/src/${_SUBDIRNAME})

unset(_SUBDIRNAME)
unset(_SUBLIBNAME)