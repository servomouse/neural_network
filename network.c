#include "network.h"
#include "stdlib.h"

neuron_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs) {
    neuron_t *net = malloc(sizeof(neuron_t) * (num_inputs + num_neurons));

    return net;
}

void save_network(neuron_t *net, char *filename) {
    return;
}

void restore_network(char *filename) {
    return;
}
