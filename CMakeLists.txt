cmake_minimum_required(VERSION 3.31)
# include(config.cmake)
project(project_name)

# Set up build directories
# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER c++)

# Remove default flags
# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")
# set(CMAKE_BUILD_TYPE "")

add_subdirectory(lib)
add_subdirectory(source)
add_subdirectory(tests)
