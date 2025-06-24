set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(network)
add_subdirectory(neuron)

add_custom_target(runtests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure  # --rerun-failed
)

# Function to create a custom target for running a specific test
function(add_runtest test_name)
    add_custom_target(run_${test_name}
        COMMAND ${CMAKE_CTEST_COMMAND} -R ${test_name} --output-on-failure
        DEPENDS ${test_name}
    )
endfunction()

# Function to create tests and their corresponding targets
function(create_tests)

    set(tests_list ${ARGN})

    # Iterate over the list of tests and create individual targets
    foreach(test_name IN LISTS tests_list)
        add_dependencies(runtests ${test_name})
        message(STATUS "Creating the run_${test_name} target")
        add_runtest(${test_name})
    endforeach()
endfunction()

create_tests(
    utils_test_work_with_folders
    neuron_test_output
    neuron_test_mutation
    neuron_test_rollback
    neuron_test_save_restore
    neuron_linear_test
    neuron_poly_test
    save_restore_test
    # micronet_test
    # micronet_test_save_restore
    network_instantiate_test
    network_test_micronet
    network_test_save_restore
    neuron_poly_test_adaptation
)
