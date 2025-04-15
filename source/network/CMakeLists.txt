add_library(network
    network.c
    network_train.c
)

target_link_libraries(network
    PRIVATE
        nm_iface
        micronet
        neuron
        utils
)

target_include_directories(network
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/network
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(network
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(network
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)
