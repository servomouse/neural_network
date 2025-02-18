add_library(micronet
    micronet.c
    neuron_iface.c
)

target_link_libraries(micronet
    PRIVATE
        neuron
        utils
)

target_include_directories(micronet
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source/micronet
        ${CMAKE_CURRENT_SOURCE_DIR}
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

# target_compile_definitions(micronet PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)
