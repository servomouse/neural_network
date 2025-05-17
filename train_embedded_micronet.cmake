add_executable(train_embedded_micronets train_embedded_micronets.c)

target_link_libraries(train_embedded_micronets
    PRIVATE
        network
        neuron
        utils
)
target_include_directories(train_embedded_micronets
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(train_embedded_micronets
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(train_embedded_micronets
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)