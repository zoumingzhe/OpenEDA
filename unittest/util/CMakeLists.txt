# make unittest_geo target

set(TARGET unittest_hv_tree)

add_executable(${TARGET} 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/util/hv_tree.cpp
  hv_tree.cpp)
target_include_directories(${TARGET} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/../.. 
  ${PROJECT_SOURCE_DIR}/thirdparty/googletest/googletest/include
  ${Boost_INCLUDE_DIRS})
# add linking targets as well 
target_link_libraries(${TARGET} 
  gtest)

install(TARGETS ${TARGET} 
  RUNTIME DESTINATION unittest
  )

#add_test(NAME ${TARGET} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
#  ${CMAKE_CURRENT_SOURCE_DIR})
