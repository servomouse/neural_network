set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(micronet)
add_subdirectory(network)
add_subdirectory(neuron)

add_custom_target(runtests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS
        neuron_simple_test
        save_restore_test
)
