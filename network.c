#include "network.h"
#include "stdlib.h"

#define INPUTS_NUM 8

network_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs) {
    network_t *net = malloc(sizeof(network_t));
    net->num_inputs = num_inputs;
    net->num_outputs = num_outputs;
    net->net_size = net->num_inputs + net->num_outputs;
    net->neurons = malloc(sizeof(neuron_t) * net->net_size);
    for(uint32_t i=0; i<net->net_size; i++) {
        if(i < net->net_size)
            init_neuron(&net->neurons[i], net->neurons, 0, i);
        else
            init_neuron(&net->neurons[i], net->neurons, INPUTS_NUM, i);
    }

    return net;
}

void save_network(neuron_t *net, char *filename) {
    return;
}

void restore_network(char *filename) {
    return;
}
