add_subdirectory(greedy_legalize)
add_subdirectory(macro_legalize)
add_subdirectory(abacus_legalize)
add_subdirectory(legality_check)
add_subdirectory(k_reorder)
add_subdirectory(independent_set_matching)
add_subdirectory(global_swap)
add_subdirectory(lpdp_flow)

file(GLOB INSTALL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
install(
    FILES ${INSTALL_SRCS} DESTINATION dreamplace/ops/lp_dp
    )
