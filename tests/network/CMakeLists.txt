# Full netowrk test
# add_executable(network_test network_test.c)

# target_link_libraries(network_test
#     PRIVATE
#         network
#         micronet
#         neuron
#         utils
# )

# target_include_directories(network_test
#     PRIVATE
#         ${TESTS_DIR}
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")

# target_compile_options(network_test
#     PRIVATE
#         -Wall
#         -Wextra
#         -lm
#         -g
#         -lws2_32
#         -fstack-protector-all
# )

# target_compile_definitions(network_test
#     PRIVATE
#         BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
# )

# add_test(NAME network_test COMMAND network_test)

# Network test micronet

add_executable(network_test_micronet network_test_micronet.c)

target_link_libraries(network_test_micronet
    PRIVATE
        network
        # micronet
        neuron
        utils
)

target_include_directories(network_test_micronet
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(network_test_micronet
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(network_test_micronet
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

add_test(NAME network_test_micronet COMMAND network_test_micronet)

# Network instantiation test

add_executable(network_instantiate_test network_instantiate_test.c)

target_link_libraries(network_instantiate_test
    PRIVATE
        network
        # micronet
        neuron
        utils
)

target_include_directories(network_instantiate_test
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(network_instantiate_test
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(network_instantiate_test
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

add_test(NAME network_instantiate_test COMMAND network_instantiate_test)

# Network test save-restore

add_executable(network_test_save_restore network_test_save_restore.c)

target_link_libraries(network_test_save_restore
    PRIVATE
        network
        # micronet
        neuron
        utils
)

target_include_directories(network_test_save_restore
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(network_test_save_restore
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(network_test_save_restore
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

add_test(NAME network_test_save_restore COMMAND network_test_save_restore)
