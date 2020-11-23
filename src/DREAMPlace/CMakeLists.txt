cmake_minimum_required(VERSION 3.8.2)

project(DREAMPlace)
  
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND (NOT CMAKE_PLACE))
    set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Prefix prepended to install directories" FORCE )
endif()
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(NOT CMAKE_CXX_ABI)
    set(CMAKE_CXX_ABI 0 CACHE STRING
        "Choose the value for _GLIBCXX_USE_CXX11_ABI, options are: 0|1."
        FORCE)
endif(NOT CMAKE_CXX_ABI)
message(STATUS "CMAKE_CXX_ABI: _GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI}")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)

find_program(PYTHON "python" REQUIRED)
find_package(ZLIB REQUIRED)
set(Boost_NO_BOOST_CMAKE TRUE)
find_package(Boost 1.55.0 REQUIRED)
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
find_package(CUDA 9.0)
find_package(Cairo)
message(STATUS "Cairo: ${CAIRO_INCLUDE_DIRS}")

get_filename_component(OPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/ops ABSOLUTE)
get_filename_component(UTILITY_LIBRARY_DIRS ${CMAKE_CURRENT_BINARY_DIR}/dreamplace/ops/utility ABSOLUTE)
message(STATUS "OPS_DIR ${OPS_DIR}")
message(STATUS "UTILITY_LIBRARY_DIRS ${UTILITY_LIBRARY_DIRS}")

# thirdparty libraries 
# flute for steiner tree generation 
find_path(FLUTE_INCLUDE_DIRS flute.h PATHS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/*)
string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} FLUTE_LINK_DIRS ${FLUTE_INCLUDE_DIRS})
message(STATUS "FLUTE_INCLUDE_DIRS ${FLUTE_INCLUDE_DIRS}")
message(STATUS "FLUTE_LINK_DIRS ${FLUTE_LINK_DIRS}")

# Limbo for parsers 
set(LIMBO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Limbo)
message(STATUS "LIMBO_SOURCE_DIR ${LIMBO_SOURCE_DIR}")
set(LIMBO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/Limbo)
message(STATUS "LIMBO_BINARY_DIR ${LIMBO_BINARY_DIR}")
set(CUB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cub)
message(STATUS "CUB_DIR ${CUB_DIR}")

# munkres-cpp for Hungarian algorithm 
set(MUNKRES_CPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/munkres-cpp/src)
set(MUNKRES_CPP_LINK_DIRS ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/munkres-cpp)
message(STATUS "MUNKRES_CPP_INCLUDE_DIRS ${MUNKRES_CPP_INCLUDE_DIRS}")
message(STATUS "MUNKRES_CPP_LINK_DIRS ${MUNKRES_CPP_LINK_DIRS}")

# lemon for network flow algorithms 
find_path(LEMON_SOURCE_DIR lemon PATHS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Limbo/limbo/thirdparty/* 
	NO_DEFAULT_PATH
	)
set(LEMON_INCLUDE_DIRS "${LEMON_SOURCE_DIR}")
string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} LEMON_BINARY_DIR ${LEMON_SOURCE_DIR})
list(APPEND LEMON_INCLUDE_DIRS "${LEMON_BINARY_DIR}")
set(LEMON_LINK_DIRS ${LEMON_BINARY_DIR}/lemon)
message(STATUS "LEMON_INCLUDE_DIRS ${LEMON_INCLUDE_DIRS}")
message(STATUS "LEMON_LINK_DIRS ${LEMON_LINK_DIRS}")

if (CUDA_FOUND)
if (NOT CMAKE_CUDA_FLAGS)
    set(CMAKE_CUDA_FLAGS "-arch=sm_60"
        "-gencode=arch=compute_60,code=sm_60"
        "-gencode=arch=compute_61,code=sm_61"
        "-gencode=arch=compute_70,code=sm_70"
        )
    if (${CUDA_VERSION_MAJOR} VERSION_GREATER "9")
        list(APPEND CMAKE_CUDA_FLAGS 
            "-gencode=arch=compute_75,code=sm_75"
            "-gencode=arch=compute_75,code=compute_75"
            )
    endif(${CUDA_VERSION_MAJOR} VERSION_GREATER "9")
endif(NOT CMAKE_CUDA_FLAGS)
add_definitions(-D_CUDA_FOUND)
endif(CUDA_FOUND)
message(STATUS "CMAKE_CUDA_FLAGS: ${CMAKE_CUDA_FLAGS}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/configure.py.in ${CMAKE_CURRENT_BINARY_DIR}/dreamplace/configure.py)

add_subdirectory(thirdparty)
add_subdirectory(dreamplace)
add_subdirectory(unitest)
add_subdirectory(benchmarks)
add_subdirectory(test)

install(
    CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_INSTALL_PREFIX}/__init__.py)"
    )

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/dreamplace/configure.py DESTINATION dreamplace
    )

# new added for open-edi
if (CMAKE_PLACE)
    if (FALSE AND CUDA_FOUND)
        file(GLOB CUDA_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/ops/flow/src/*.cu"
            )
        set(CUDA_LINKED ${PROJECT_NAME}_cuda_linked)
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
        set_target_properties( ${CUDA_LINKED}
            PROPERTIES CUDA_SEPARABLE_COMPILATION ON
            )
    endif(CUDA_FOUND)
    
    file(GLOB_RECURSE SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/ops/tcl_command/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/ops/utility/*.cpp
        )
    link_directories(
        ${PROJECT_BINARY_DIR}/src/db
        ${PROJECT_BINARY_DIR}/src/parser
        ${PROJECT_BINARY_DIR}/src/util
        ${PROJECT_BINARY_DIR}/src/gui
        ${PROJECT_BINARY_DIR}/src/console
        ${TCL_DIR}/lib
        ${Boost_LIBRARY_DIRS}
        ${PROFILER_DIR}/lib
        ${LIMBO_BINARY_DIR}
        ${MUNKRES_CPP_LINK_DIRS}
        ${LEMON_INCLUDE_DIRS}
        )
        
    add_library(${PROJECT_NAME_LOWERCASE}_DREAMPlace STATIC 
        ${SRCS}
        )
    target_link_libraries(${PROJECT_NAME_LOWERCASE}_DREAMPlace PUBLIC
        tcl_command place_flow place_abacus_legalize place_global_swap place_greedy_legalize
        place_independent_set_matching place_k_reorder place_legality_check
        place_macro_legalize
        )
    target_include_directories(${PROJECT_NAME_LOWERCASE}_DREAMPlace PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/dreamplace/ops
        ${LIMBO_SOURCE_DIR}
        ${Boost_HEADERS} 
        ${PROFILER_DIR}/include
        ${TCL_DIR}/include
        ${CUDA_LINKED} PUBLIC
        ${CUB_DIR}
        ${Boost_INCLUDE_DIRS}
        ${MUNKRES_CPP_INCLUDE_DIRS}
        ${LEMON_INCLUDE_DIRS}
        )
endif(CMAKE_PLACE)
# end of open-edi
