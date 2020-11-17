# Make the main binary

set(_SUBDIRNAME app)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "openeda")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
file(GLOB_RECURSE SRCS *.cpp)
if (CMAKE_PLACE) 
  add_definitions(-D_MAKE_PLACE)
  link_directories(
      ${PROJECT_BINARY_DIR}/src/db
      ${PROJECT_BINARY_DIR}/src/parser
      ${PROJECT_BINARY_DIR}/src/util
      ${PROJECT_BINARY_DIR}/src/gui
      ${PROJECT_BINARY_DIR}/src/console
      ${TCL_DIR}/lib
      ${Boost_LIBRARY_DIRS}
      ${PROFILER_DIR}/lib
      ${PROJECT_BINARY_DIR}/src/DREAMPlace
      )
else (CMAKE_PLACE)
  link_directories(
      ${PROJECT_BINARY_DIR}/src/db
      ${PROJECT_BINARY_DIR}/src/parser
      ${PROJECT_BINARY_DIR}/src/util
      ${PROJECT_BINARY_DIR}/src/gui
      ${PROJECT_BINARY_DIR}/src/console
      ${TCL_DIR}/lib
      ${Boost_LIBRARY_DIRS}
      ${PROFILER_DIR}/lib
      )
endif(CMAKE_PLACE)

 
# set RPATH

set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/share")  
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/share")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH  TRUE)

add_executable(${_SUBBINNAME} app_main.cpp)

if (CMAKE_PLACE) 
  target_link_libraries(${_SUBBINNAME} PUBLIC 
      openedi_tcl openedi_db openedi_util z boost_system boost_filesystem openedi_parser
      openedi_lefrw openedi_lef openedi_defrw openedi_def openedi_tcl openedi_verilog
      tcl8.6 profiler tcmalloc pthread openedi_gui openedi_DREAMPlace)
  
  target_include_directories(${_SUBBINNAME} PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/.
      ${CMAKE_CURRENT_SOURCE_DIR}/../DREAMPlace/dreamplace/ops
      ${PROFILER_DIR}/include
      ${Boost_HEADERS}
      ${TCL_DIR}/include)
else (CMAKE_PLACE)
  target_link_libraries(${_SUBBINNAME} PUBLIC 
      openedi_tcl openedi_db openedi_util z boost_system boost_filesystem openedi_parser
      openedi_lefrw openedi_lef openedi_defrw openedi_def openedi_tcl openedi_verilog
      tcl8.6 profiler tcmalloc pthread openedi_gui)
  
  target_include_directories(${_SUBBINNAME} PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/.
      ${PROFILER_DIR}/include
      ${Boost_HEADERS}
      ${TCL_DIR}/include)
endif(CMAKE_PLACE)


#target_link_libraries(${_SUBBINNAME} PUBLIC 
#    ${_SUBLIBNAME} openedi_db openedi_util boost_system boost_filesystem openedi_parser
#    openedi_lefrw openedi_lef openedi_defrw openedi_def openedi_tcl verilogparser
#    tcl8.6 profiler tcmalloc)

#add_library(${_SUBLIBNAME} app_test.cpp)
#target_include_directories(${_SUBLIBNAME} PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}/../..
#    ${PROFILER_DIR}/include
#    ${Boost_HEADERS}
#    ${TCL_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-g)
endif()

# must turn fPIC on 
set_target_properties(${_SUBBINNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# install share library 
#install( FILES ${TCL_DIR}/lib/libtcl8.6.so DESTINATION lib/share )

install(TARGETS ${_SUBBINNAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/share
  ARCHIVE DESTINATION lib/static
  )

unset(_SUBDIRNAME)
unset(_SUBLIBNAME)
unset(_SUBBINNAME)
