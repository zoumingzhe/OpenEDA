project(place_flow)

if (CUDA_FOUND)
    add_definitions(-D_CUDA_FOUND)
    file(GLOB CUDA_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu"
        )
    set(CUDA_LINKED ${PROJECT_NAME}_cuda_linked)
    # try to turn on -fPIC for cuda, but not really working
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CUDA_SEPARABLE_COMPILATION TRUE)
    list(APPEND CUDA_NVCC_FLAGS
        -O3 -std=c++14 ${CMAKE_CUDA_FLAGS} 
        )
    list(APPEND CUDA_NVCC_FLAGS --compiler-options -fPIC)

    cuda_add_library(
        ${CUDA_LINKED} STATIC
        ${CUDA_SOURCES}
        )

    target_include_directories(
        ${CUDA_LINKED} PUBLIC 
        ${OPS_DIR}
	${LIMBO_SOURCE_DIR}
        ${CUB_DIR}
        ${Boost_INCLUDE_DIRS}
        )

    # Request that cuda_linked be built with -std=c++14
    # As this is a public compile feature anything that links to cuda_linked
    # will also build with -std=c++14
	#target_compile_features(${CUDA_LINKED} PUBLIC cxx_std_11)

    # We need to explicitly state that we need all CUDA files in the cuda_linked
    # library to be built with -dc as the member functions could be called by
    # other libraries and executables
    set_target_properties( ${CUDA_LINKED}
        PROPERTIES CUDA_SEPARABLE_COMPILATION ON
        )
endif(CUDA_FOUND)
configure_file(src/global_place.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/global_place.cpp)

file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/.." 
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
include_directories("${LIMBO_SOURCE_DIR}")
include_directories("${Boost_INCLUDE_DIRS}")
include_directories("${Python_INCLUDE_DIRS}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
#add_executable(test_p ${SOURCES})
#target_link_libraries(test_p ${UTILITY_LIBRARY_DIRS}/libutility.a)
#target_link_libraries(test_p ${Python_LIBRARIES})

link_directories(${Python_LIBRARY_DIRS})
link_directories(${Python_SITELIB})
link_directories(${UTILITY_LIBRARY_DIRS})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

if (CUDA_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${Python_LIBRARIES} ${CUDA_LINKED})
else (CUDA_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${Python_LIBRARIES})
endif(CUDA_FOUND)
#install ( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/test_p DESTINATION dreamplace )


