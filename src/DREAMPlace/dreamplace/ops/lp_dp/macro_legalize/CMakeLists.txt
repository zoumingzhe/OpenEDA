project(place_macro_legalize)

file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../.."
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.."
                    "${LEMON_INCLUDE_DIRS}")
include_directories(${LIMBO_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${UTILITY_LIBRARY_DIRS})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC utility)
