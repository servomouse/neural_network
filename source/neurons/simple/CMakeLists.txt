set(neuron_simple_sources
    neuron.c
)

add_library(neuron_simple ${neuron_simple_sources})

target_link_libraries(neuron_simple
    PRIVATE
        utils
)

target_include_directories(neuron_simple
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neurons
)

target_include_directories(neuron_simple
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# add_library(neuron_simple_shared SHARED ${neuron_simple_sources})

# target_include_directories(neuron_simple
#     PRIVATE
#         ${CMAKE_SOURCE_DIR}/lib
#         ${CMAKE_SOURCE_DIR}/source/neurons
# )

# target_include_directories(neuron_simple_shared
#     PRIVATE
#         ${CMAKE_SOURCE_DIR}/lib
#         ${CMAKE_SOURCE_DIR}/source/neurons
# )

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

# set(CMAKE_C_FLAGS "-Wall -Wextra -lm -g -lws2_32 -fstack-protector-all")
# # set(CMAKE_C_OUTPUT_EXTENSION ".o")

# target_compile_options(neuron_simple
#     PRIVATE
#         ${CMAKE_C_FLAGS}
# )

target_compile_options(neuron_simple
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)
