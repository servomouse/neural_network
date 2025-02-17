add_library(neuron_test_common
    common_functions.c
)

target_link_libraries(neuron_test_common
    PRIVATE
        neuron
        utils
)

target_include_directories(neuron_test_common
    PRIVATE
        ${TESTS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test basic functions
add_executable(neuron_test_output neuron_test_output.c)

target_link_libraries(neuron_test_output
    PRIVATE
        neuron
        utils
        neuron_test_common
)
target_include_directories(neuron_test_output
    PRIVATE
        ${TESTS_DIR}
)

add_test(NAME neuron_test_output COMMAND neuron_test_output)

# Test mutations
add_executable(neuron_test_mutation neuron_test_mutation.c)

target_link_libraries(neuron_test_mutation
    PRIVATE
        neuron
        utils
        neuron_test_common
)
target_include_directories(neuron_test_mutation
    PRIVATE
        ${TESTS_DIR}
)

add_test(NAME neuron_test_mutation COMMAND neuron_test_mutation)

# Test rollback
add_executable(neuron_test_rollback neuron_test_rollback.c)

target_link_libraries(neuron_test_rollback
    PRIVATE
        neuron
        utils
        neuron_test_common
)
target_include_directories(neuron_test_rollback
    PRIVATE
        ${TESTS_DIR}
)

add_test(NAME neuron_test_rollback COMMAND neuron_test_rollback)

# Test the save-restore functionality
add_executable(neuron_test_save_restore neuron_test_save_restore.c)

target_link_libraries(neuron_test_save_restore
    PRIVATE
        neuron
        utils
        neuron_test_common
)
target_include_directories(neuron_test_save_restore
    PRIVATE
        ${TESTS_DIR}
)

add_test(NAME neuron_test_save_restore COMMAND neuron_test_save_restore)
