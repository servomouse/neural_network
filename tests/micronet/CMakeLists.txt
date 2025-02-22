# Micronet test
add_executable(micronet_test micronet_test.c)

target_link_libraries(micronet_test
    PRIVATE
        micronet
        neuron
        utils
)

target_include_directories(micronet_test
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(micronet_test
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)
        
add_test(NAME micronet_test COMMAND micronet_test)


add_executable(micronet_test_save_restore micronet_test_save_restore.c)

target_link_libraries(micronet_test_save_restore
    PRIVATE
        micronet
        neuron
        utils
)

target_include_directories(micronet_test_save_restore
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")

target_compile_options(micronet_test_save_restore
    PRIVATE
        -Wall
        -Wextra
        -lm
        -g
        -lws2_32
        -fstack-protector-all
)

target_compile_definitions(micronet_test_save_restore
    PRIVATE
        BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

add_test(NAME micronet_test_save_restore COMMAND micronet_test_save_restore)
