add_executable(neuron_simple_test neuron_simple_test.c)
target_link_libraries(neuron_simple_test
    neuron_simple
    utils
)

add_test(NAME neuron_simple_test COMMAND neuron_simple_test)

add_executable(neuron_smart_test neuron_smart_test.c)
target_link_libraries(neuron_smart_test
    neuron_smart
    utils
)


add_executable(micronet_test micronet_test.c)

add_executable(network_test network_test.c)
