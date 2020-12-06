# make a util target for utility

set(_SUBDIRNAME util)
set(_SUBLIBNAME ${PROJECT_NAME_LOWERCASE}_${_SUBDIRNAME})
set(_SUBBINNAME "")

file(GLOB_RECURSE SRCS *.cpp)
list(FILTER SRCS EXCLUDE REGEX ".*hv_tree.cpp$")

#add_library(${PROJECT_NAME_LOWERCASE}_util STATIC ${SRCS})
#target_include_directories(${PROJECT_NAME_LOWERCASE}_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${Boost_HEADERS})

link_directories(${TCL_DIR}/lib /lib64)

add_library(${_SUBLIBNAME} STATIC ${SRCS})
target_link_libraries(${_SUBLIBNAME} PUBLIC lz4 z)
target_include_directories(${_SUBLIBNAME} PUBLIC
    ${PROJECT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${TCL_DIR}/include
    ${Boost_INCLUDE_DIRS})
add_compile_options(-Wno-pointer-arith -Wint-to-pointer-cast=no)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-g)
endif()

# must turn fPIC on 
set_target_properties(${_SUBLIBNAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
install(TARGETS ${_SUBLIBNAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )

# install headers
file(GLOB_RECURSE HEADERS *.h *.hpp *.msg)
install(FILES ${HEADERS} DESTINATION include/src/${_SUBDIRNAME})

unset(_SUBDIRNAME)
unset(_SUBLIBNAME)
unset(_SUBBINNAME)
