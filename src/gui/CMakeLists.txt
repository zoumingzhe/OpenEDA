# make a gui setup
# CMake setup

add_subdirectory(console)
add_subdirectory(qtint)
add_subdirectory(win)

set(_SUBDIRNAME gui)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

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
target_link_libraries(${_SUBLIBNAME} Qt5::Widgets Qt5::Core Qt5::Gui
${PROJECT_NAME_LOWERCASE}_console 
${PROJECT_NAME_LOWERCASE}_qtint
${PROJECT_NAME_LOWERCASE}_win
)

install(TARGETS ${_SUBLIBNAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib/static
     )

# install headers 
file(GLOB HEADERS *.h *.hpp)
install(FILES ${HEADERS} DESTINATION include/src/${_SUBDIRNAME})