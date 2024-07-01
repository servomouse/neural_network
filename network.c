#include "network.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define INPUTS_NUM 4

network_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs) {
    if(num_outputs > num_neurons) {
        printf("num_neurons cannot be less than num_outputs");
        return NULL;
    }
    network_t *net = malloc(sizeof(network_t));
    net->last_mutant_idx = 0;
    net->num_inputs = num_inputs;
    net->num_outputs = num_outputs;
    net->net_size = net->num_inputs + num_neurons;
    net->neurons = malloc(sizeof(neuron_t) * net->net_size);
    for(uint32_t i=0; i<(net->net_size); i++) {
        if(i < net->num_inputs)
            init_neuron(&net->neurons[i], net->neurons, 0, i);
        else
            init_neuron(&net->neurons[i], net->neurons, INPUTS_NUM, i);
    }

    return net;
}

double get_output(network_t *net, uint32_t num_inputs, double inputs[]) {
    for(uint32_t i=0; i<num_inputs; i++) {
        net->neurons[i].output = inputs[i];
    }
    for(uint32_t i=0; i<(net->net_size); i++) {
        calc_output(&net->neurons[i], net->neurons);
        update_output(&net->neurons[i]);
    }
    return net->neurons[net->net_size-1].output;
}

void mutate(network_t *net) {
    net->last_mutant_idx = random_int(net->num_inputs, net->net_size);
    random_mutation(&net->neurons[net->last_mutant_idx]);
}

void repair(network_t *net) {
    rollback_mutation(&net->neurons[net->last_mutant_idx]);
}

void save_network(network_t *net, char *filename) {
    return;
}

void restore_network(char *filename) {
    return;
}

void print_results(network_t *net) {
    print_coeffs(&net->neurons[4], net->neurons);
}
