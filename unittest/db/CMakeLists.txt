# make unittest_db target 

set(TARGET unittest_db)
add_definitions(-g)

file(GLOB UNITTEST_SRCS *.cpp)
add_executable(${TARGET} ${UNITTEST_SRCS})
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${PROJECT_SOURCE_DIR}/thirdparty/googletest/googletest/include)
# add linking targets as well 
target_link_libraries(${TARGET} 
  ${PROJECT_NAME_LOWERCASE}_db 
  ${PROJECT_NAME_LOWERCASE}_parser 
  ${PROJECT_NAME_LOWERCASE}_util 
  gtest)

install(TARGETS ${TARGET} 
  RUNTIME DESTINATION unittest
  )

add_test(NAME ${TARGET} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
  ${CMAKE_CURRENT_SOURCE_DIR})
