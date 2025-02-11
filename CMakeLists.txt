cmake_minimum_required(VERSION 3.10)
project(project_name)

# Set up build directories
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER c++)

# Add subdirectories
add_subdirectory(source)
add_subdirectory(tests)
add_subdirectory(lib)

# Create a target to build the entire project
add_custom_target(neurons
    DEPENDS
        neuron_linear
        neuron_simple
        neuron_smart
)
