#include "micronet_mutations.h"
#include "neuron.h"

// Complementary function is micronet_rollback
void micronet_mutate(micro_network_t * config) {
    // config->mutated_neuron_idx = random_int(0, config->num_neurons);
    config->mutated_neuron_idx++;
    if(config->mutated_neuron_idx == config->num_neurons) {
        config->mutated_neuron_idx = 0;
    }
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

// Complementary function is micronet_mutate
void micronet_rollback(micro_network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}
