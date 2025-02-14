cmake_minimum_required(VERSION 3.31)
# include(config.cmake)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(project_name)

include(CTest)

# Set up build directories
# set(CMAKE_BUILD_TYPE Release)

# Remove default flags
# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")
# set(CMAKE_BUILD_TYPE "")

add_subdirectory(lib)
add_subdirectory(source)
add_subdirectory(tests)
