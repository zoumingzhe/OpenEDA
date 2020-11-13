
set(_SUBDIRNAME console)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")



file(GLOB SRC_FILES *.c)
file(GLOB HEAD_FILES *.h)


add_library(${_SUBLIBNAME} STATIC 
    ${SRC_FILES} 
    ${HEAD_FILES}
 )

target_include_directories(${_SUBLIBNAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../.. 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROFILER_DIR}/include
    ${Boost_HEADERS} 
    ${TCL_DIR}/include
)

set_target_properties(${_SUBLIBNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${_SUBLIBNAME} ${TCL_LIBRARY} readline)

install(TARGETS ${_SUBLIBNAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib/static
     )

# install headers 
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/src/gui/${_SUBDIRNAME})
# install scripts
file(GLOB_RECURSE SCRIPTS ./lib/*.tcl ./lib/*.tclshrc)
# install(FILES ${SCRIPTS} DESTINATION include/src/gui/${_SUBDIRNAME}/lib)
install(FILES ${SCRIPTS} DESTINATION share/etc/tcl)



unset(_SUBDIRNAME)
unset(_SUBLIBNAME)
unset(_SUBBINNAME)



