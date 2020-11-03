# make thirdparty targets 

# googletest for testing
option(BUILD_GMOCK "" OFF)
option(INSTALL_GTEST "" OFF)
add_subdirectory(googletest)

# pybind11 for python binding 
add_subdirectory(pybind11)
