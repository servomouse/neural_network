#include "network_mutations.h"
#include "neuron.h"

// Complementary function is network_rollback
void network_mutate(network_t * config) {
    // config->mutated_neuron_idx = random_int(0, config->num_neurons);
    config->mutated_neuron_idx++;
    if(config->mutated_neuron_idx == config->num_neurons) {
        config->mutated_neuron_idx = 0;
    }
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

// Complementary function is network_mutate
void network_rollback(network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}
