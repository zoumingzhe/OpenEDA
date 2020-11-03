# make a python target for python binding 

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${Boost_INCLUDE_DIRS})
file(GLOB SRCS *.cpp)

pybind11_add_module(edi_python ${SRCS})
target_include_directories(edi_python PUBLIC ${HEADERS})
target_link_libraries(edi_python PUBLIC 
  ${PROJECT_NAME_LOWERCASE}_db 
  ${PROJECT_NAME_LOWERCASE}_parser
  ${PROJECT_NAME_LOWERCASE}_util
  )
set_target_properties(edi_python PROPERTIES OUTPUT_NAME ${PROJECT_NAME_LOWERCASE})
install(TARGETS edi_python 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
  )
