set(_SUBDIRNAME qtint)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")

find_package(Qt5Gui REQUIRED)
set(CMAKE_AUTOMOC ON)

file(GLOB SRC_FILES *.cpp)
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
target_link_libraries(${_SUBLIBNAME} ${TCL_LIBRARY} Qt5::Gui)

install(TARGETS ${_SUBLIBNAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib/static
     )

# install headers 
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/src/gui/${_SUBDIRNAME})


unset(_SUBDIRNAME)
unset(_SUBLIBNAME)
unset(_SUBBINNAME)