set(_SUBDIRNAME win)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE HEAD_FILES *.h)
file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE QRC_FILES *.qrc)

add_library(${_SUBLIBNAME} STATIC 
    ${SRC_FILES} 
    ${HEAD_FILES}
    ${UI_FILES}
    ${QRC_FILES}
)

target_include_directories(${_SUBLIBNAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../.. 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROFILER_DIR}/include
    ${Boost_HEADERS} 
    ${TCL_DIR}/include
)

set_target_properties(${_SUBLIBNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${_SUBLIBNAME} ${TCL_LIBRARY} Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS ${_SUBLIBNAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib/static
     )

# install headers 
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/src/gui/win/${_SUBDIRNAME})


unset(_SUBDIRNAME)
unset(_SUBLIBNAME)
unset(_SUBBINNAME)