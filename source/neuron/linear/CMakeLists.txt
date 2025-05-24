set(LINEAR_NEURON_SOURCES
    linear_neuron.c
    linear_neuron_mutations.c
    linear_neuron_save_restore.c
)

add_library(linear_neuron ${LINEAR_NEURON_SOURCES})

target_link_libraries(linear_neuron
    PRIVATE
        utils
)

target_include_directories(linear_neuron
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neuron
        ${PROJECT_SOURCE_DIR}/source/neuron/linear
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(linear_neuron
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(linear_neuron
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)
