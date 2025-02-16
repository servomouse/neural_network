# Single neuron test
add_executable(neuron_simple_test neuron_simple_test.c)
target_link_libraries(neuron_simple_test
    neuron
    utils
)

add_test(NAME neuron_simple_test COMMAND neuron_simple_test)


# add_executable(neuron_smart_test neuron_smart_test.c)
# target_link_libraries(neuron_smart_test
#     neuron
#     utils
# )

# Micronet test
add_executable(micronet_test micronet_test.c)

# Full netowrk test
add_executable(network_test network_test.c)

# Save-restore functionality test
add_executable(save_restore_test save_restore_test.c)
target_link_libraries(save_restore_test
    utils
)

add_test(NAME save_restore_test COMMAND save_restore_test)

add_custom_target(runtests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS
        neuron_simple_test
        save_restore_test
)
