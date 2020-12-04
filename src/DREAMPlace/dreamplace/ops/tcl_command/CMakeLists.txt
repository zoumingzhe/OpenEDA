project(place_tcl_command)

file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../.."
    "${TCL_DIR}/include"
    "${Boost_INCLUDE_DIRS}"
    "${PROFILER_DIR}/include"
    )

link_directories(${UTILITY_LIBRARY_DIRS})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC utility)
