add_library(micronet
    micronet.c
    neuron_iface.c
)

target_link_libraries(micronet
    neuron
    utils
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(micronet
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(micronet
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

target_compile_definitions(micronet PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)
