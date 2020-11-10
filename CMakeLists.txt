# CMake setup
cmake_minimum_required (VERSION 3.9)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(OpenEDI)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dev tool path
if (DEFINED ENV{DEV_TOOL})   #find env
    MESSAGE(STATUS "Found DEV_TOOL env = " $ENV{DEV_TOOL})
    set(OPT_PATH $ENV{DEV_TOOL})
else (NOT DEFINED ENV{DEV_TOOL})  #find defautl
    set (OPT_PATH "/home/opt/devtoolset")
endif()

MESSAGE(STATUS "DEV_TOOL path: " ${OPT_PATH})

# set Boost path
find_package(Boost 1.55.0 REQUIRED)
if (Boost_FOUND)
    set(Boost_HEADERS  Boost::header ${Boost_INCLUDE_DIRS})
else ()
    set(Boost_DIR ${OPT_PATH}/boost-1.69)
    set(Boost_HEADERS ${Boost_DIR}/include)
    set(Boost_LIBRARY_DIRS ${Boost_DIR}/lib)
endif ()
message(STATUS "Boost_HEADERS = ${Boost_HEADERS}")
message(STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}")

# set Tcl path

set(TCL_DIR ${OPT_PATH}/tcl-8.6.9)
message(STATUS "TCL_DIR = ${TCL_DIR}")

#find_package(TCL 8.6.9 REQUIRED)  // cannot use find_package as the system tclsh will be find
#if (TCL_FOUND)
    #set(Tcl_DIR Tcl::headers ${Tcl_INCLUDE_DIRS})
    
#else (TCL_FOUND)

#endif (TCL_FOUND)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

# set gperf path
set(PROFILER_DIR ${OPT_PATH}/gperftools-2.7.90)
message(STATUS "PROFILER_DIR = ${PROFILER_DIR}")

find_package(BISON 3.0 REQUIRED)
find_package(FLEX REQUIRED)
#find_package(TCL)
# use Doxygen to generate documentation 
find_package(Doxygen)

#option(EDI_BIND_PYTHON "whether bind python" ON)
#if (TCL_FOUND)
#  message(STATUS "TCL_INCLUDE_PATH = ${TCL_INCLUDE_PATH}")
#  set(EDI_BIND_TCL ON)
#  #option(EDI_BIND_TCL "whether bind tcl" OFF)
#else(TCL_FOUND)
#  set(EDI_BIND_TCL OFF)
#endif(TCL_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreturn-type  -Wno-attributes")

message(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " ${CMAKE_EXE_LINKER_FLAGS})
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "EDI_BIND_PYTHON: ${EDI_BIND_PYTHON}")
message(STATUS "EDI_BIND_TCL: ${EDI_BIND_TCL}")

# without this, clang will complain about linking 
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# testing are defined in unittest directory 
enable_testing()

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)
message(STATUS "GCOV: ${ENABLE_GCOV}")
IF (ENABLE_GCOV)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
ENDIF()

option(CMAKE_USE_PYTHON2 "Whether use python2 instead of python3" OFF)
if(CMAKE_USE_PYTHON2)
  find_package(PythonInterp 2)
else(CMAKE_USE_PYTHON2)
  find_package(PythonInterp 3)
  # if not found, search python2 instead 
  if (NOT PYTHONINTERP_FOUND)
    find_package(PythonInterp 2)
  endif(NOT PYTHONINTERP_FOUND)
endif(CMAKE_USE_PYTHON2)
add_subdirectory(thirdparty)

# python version is determined by pybind11 in thirdparty 
message(STATUS "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_VERSION: ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
option(CMAKE_PLACE "Whether compile place" TRUE)

add_subdirectory(src)
add_subdirectory(unittest)
add_subdirectory(demo)
add_subdirectory(docs)

# create an empty init script for python module  
# this is to support python 2.7, while python3 no longer needs this
install(CODE "execute_process(COMMAND touch ${CMAKE_INSTALL_PREFIX}/lib/__init__.py)")
