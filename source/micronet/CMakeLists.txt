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
