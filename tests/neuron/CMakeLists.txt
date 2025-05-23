
# Common functions:
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

macro(add_neuron_test TEST_NAME)
    message(STATUS "Creating target ${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST_NAME}.c)
    
    target_link_libraries(${TEST_NAME}
        PRIVATE
            neuron
            utils
            neuron_test_common
    )
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${TESTS_DIR}
            ${PROJECT_SOURCE_DIR}
    )
    
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

# Test basic functions
add_neuron_test(neuron_test_output)

# Test mutations
add_neuron_test(neuron_test_mutation)

# Test rollback
add_neuron_test(neuron_test_rollback)

# Test the save-restore functionality
add_neuron_test(neuron_test_save_restore)

# Test linear neuron functionality
add_neuron_test(neuron_linear_test)

# Test poly neuron functionality
add_neuron_test(neuron_poly_test)

# Test poly neuron adaptation
add_neuron_test(neuron_poly_test_adaptation)
