add_subdirectory(ops)

file(GLOB INSTALL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.py" "${CMAKE_CURRENT_SOURCE_DIR}/*.json")
install(
    FILES ${INSTALL_SRCS} DESTINATION dreamplace
    )
