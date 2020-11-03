# def
add_subdirectory(def)
# lef
add_subdirectory(lef)
# verilog
add_subdirectory(verilog)

file(GLOB SRCS 
  graphviz/*.cpp
  )

add_library(${PROJECT_NAME_LOWERCASE}_parser STATIC ${SRCS})
target_include_directories(${PROJECT_NAME_LOWERCASE}_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${Boost_INCLUDE_DIRS})
# must turn fPIC on 
set_target_properties(${PROJECT_NAME_LOWERCASE}_parser PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
install(TARGETS ${PROJECT_NAME_LOWERCASE}_parser 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )

