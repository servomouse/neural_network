set(neuron_sources
    neuron.c
)

add_library(neuron ${neuron_sources})

target_link_libraries(neuron
    PRIVATE
        utils
)

target_include_directories(neuron
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neuron
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(neuron
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)