add_library(nm_iface
    neuron_micronet_iface.c
)

target_link_libraries(nm_iface
    PRIVATE
        neuron
        micronet
        utils
)

target_include_directories(nm_iface
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/neuron-micronet_inteface
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(nm_iface
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(nm_iface
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

# target_compile_definitions(micronet PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)
