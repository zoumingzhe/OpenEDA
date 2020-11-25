# make a db object 
# CMake setup
cmake_minimum_required (VERSION 3.9)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(TIMING_HOME ${PROJECT_SOURCE_DIR}/src/db/timing)
set(TIMINGLIB_HOME ${TIMING_HOME}/timinglib)
set(TIMINGSPEF_HOME ${TIMING_HOME}/spef)

BISON_TARGET(timinglib_libparser_parser
             ${TIMINGLIB_HOME}/timinglib_libparser.y
             ${CMAKE_CURRENT_BINARY_DIR}/timinglib_libparser.tab.cc)
FLEX_TARGET(timinglib_libparser_lexer
            ${TIMINGLIB_HOME}/timinglib_libparser.l
            ${CMAKE_CURRENT_BINARY_DIR}/timinglib_libparser.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(timinglib_libparser_lexer timinglib_libparser_parser)

BISON_TARGET(timinglib_syntaxparser_parser
            ${TIMINGLIB_HOME}/timinglib_syntaxparser.y
            ${CMAKE_CURRENT_BINARY_DIR}/timinglib_syntaxparser.tab.cc)
FLEX_TARGET(timinglib_syntaxparser_lexer
            ${TIMINGLIB_HOME}/timinglib_syntaxparser.l
            ${CMAKE_CURRENT_BINARY_DIR}/timinglib_syntaxparser.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(timinglib_syntaxparser_lexer timinglib_syntaxparser_parser)

BISON_TARGET(spef_parser_parser
            ${TIMINGSPEF_HOME}/spef_parser.y
            ${CMAKE_CURRENT_BINARY_DIR}/spef_parser.yacc.cc)
FLEX_TARGET(spef_parser_lexer
            ${TIMINGSPEF_HOME}/spef_parser.l
            ${CMAKE_CURRENT_BINARY_DIR}/spef_parser.lex.cc)
ADD_FLEX_BISON_DEPENDENCY(spef_parser_lexer spef_parser_parser)

file(GLOB_RECURSE SRCS *.cpp)

# link_directories(${PROJECT_BINARY_DIR}/src/io/def/def)

add_library(${PROJECT_NAME_LOWERCASE}_db STATIC 
  ${SRCS}
  ${FLEX_timinglib_libparser_lexer_OUTPUTS}
  ${BISON_timinglib_libparser_parser_OUTPUTS}

  ${FLEX_timinglib_syntaxparser_lexer_OUTPUTS}
  ${BISON_timinglib_syntaxparser_parser_OUTPUTS}

  ${FLEX_spef_parser_lexer_OUTPUTS}
  ${BISON_spef_parser_parser_OUTPUTS}
  
)
target_include_directories(${PROJECT_NAME_LOWERCASE}_db PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    ${CMAKE_CURRENT_SOURCE_DIR}/../parser/verilog
    ${TCL_DIR}/include 
    ${Boost_INCLUDE_DIRS} 
    ${PROFILER_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/timing/timinglib
    ${CMAKE_CURRENT_SOURCE_DIR}/timing/spef)

# for yosys parser verilog
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_YOSYS_ -DYOSYS_ENABLE_ZLIB")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_YOSYS_ -DYOSYS_ENABLE_ZLIB")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_YOSYS_ -DYOSYS_ENABLE_ZLIB")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_YOSYS_ -DYOSYS_ENABLE_ZLIB")
set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -D_YOSYS_ -DYOSYS_ENABLE_ZLIB")

# must turn fPIC on 
set_target_properties(${PROJECT_NAME_LOWERCASE}_db PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
install(TARGETS ${PROJECT_NAME_LOWERCASE}_db 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )

# install headers 
file(GLOB_RECURSE HEADERS1 core/*.h core/*.hpp)
install(FILES ${HEADERS1} DESTINATION include/src/db/core)
file(GLOB_RECURSE HEADERS2 io/*.h io/*.hpp)
install(FILES ${HEADERS2} DESTINATION include/src/db/io)
file(GLOB_RECURSE HEADERS3 tech/*.h tech/*.hpp)
install(FILES ${HEADERS3} DESTINATION include/src/db/tech)
file(GLOB_RECURSE HEADERS4 timing/*.h timing/*.hpp)
install(FILES ${HEADERS4} DESTINATION include/src/db/timing)
file(GLOB_RECURSE HEADERS5 timing/timinglib/*.msg)
install(FILES ${HEADERS5} DESTINATION include/src/db/timing/timinglib)
file(GLOB_RECURSE HEADERS6 util/*.h util/*.hpp)
install(FILES ${HEADERS6} DESTINATION include/src/db/util)
file(GLOB_RECURSE HEADERS7 timing/spef/*.msg)
install(FILES ${HEADERS7} DESTINATION include/src/db/timing/spef)
