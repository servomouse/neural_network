add_executable(neuron_linear_test neuron_linear_test.c)
target_link_libraries(neuron_linear_test
    neuron_linear
    utils
)

add_executable(neuron_simple_test neuron_simple_test.c)
target_link_libraries(neuron_simple_test
    neuron_simple
    utils
)

add_executable(neuron_smart_test neuron_smart_test.c)
target_link_libraries(neuron_smart_test neuron_smart)


add_executable(micronet_test micronet_test.c)

add_executable(network_test network_test.c)

# Link the tests executable against the module libraries
# target_link_libraries(tests
#   module1
#   module2
#   module3
# )