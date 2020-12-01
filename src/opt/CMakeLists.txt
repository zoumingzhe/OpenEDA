# make a opt object 

set(_SUBDIRNAME opt)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")

file(GLOB_RECURSE SRCS *.cpp)

add_library(${_SUBLIBNAME} STATIC ${SRCS})
target_include_directories(${_SUBLIBNAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    ${PROFILER_DIR}/include
    ${TCL_DIR}/include)


set_target_properties(${_SUBLIBNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
install(TARGETS ${_SUBLIBNAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )

# install headers 
file(GLOB_RECURSE HEADERS *.h *.hpp)
install(FILES ${HEADERS} DESTINATION include/src/${_SUBDIRNAME})