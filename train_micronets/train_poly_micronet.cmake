add_executable(train_poly_micronet train_poly_micronet.c)

target_link_libraries(train_poly_micronet
    PRIVATE
        network
        neuron
        utils
)

target_include_directories(train_poly_micronet
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(train_poly_micronet
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(train_poly_micronet
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)
