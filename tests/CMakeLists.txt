add_executable(tests
  micronet_test.c
  network_test.c
  neuron_test.c
)

# Link the tests executable against the module libraries
target_link_libraries(tests
  module1
  module2
  module3
)