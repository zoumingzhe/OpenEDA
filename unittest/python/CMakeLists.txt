# python unittest 

file(GLOB SRCS *.py *.txt)
install(FILES ${SRCS} DESTINATION unittest/python)

add_test(NAME unittest_python_geometry COMMAND ${PYTHON_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/unittest_geo.py
  ${PROJECT_BINARY_DIR}/src/python)

add_test(NAME unittest_python_utility COMMAND ${PYTHON_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/unittest_util.py
  ${PROJECT_BINARY_DIR}/src/python)

add_test(NAME unittest_python_database COMMAND ${PYTHON_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/unittest_db.py
  ${PROJECT_BINARY_DIR}/src/python)