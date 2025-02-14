set(neuron_smart_sources
    neuron.c
)

add_library(neuron_smart ${neuron_smart_sources})

target_link_libraries(neuron_smart
    PRIVATE
        utils
)

target_include_directories(neuron_smart
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neurons
)

target_include_directories(neuron_smart
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(neuron_smart
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)