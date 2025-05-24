set(POLY_NEURON_SOURCES
    poly_neuron.c
    poly_neuron_mutations.c
    poly_neuron_save_restore.c
)

add_library(poly_neuron ${POLY_NEURON_SOURCES})

target_link_libraries(poly_neuron
    PRIVATE
        utils
)

target_include_directories(poly_neuron
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neuron
        ${PROJECT_SOURCE_DIR}/source/neuron/poly
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(poly_neuron
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(poly_neuron
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)
