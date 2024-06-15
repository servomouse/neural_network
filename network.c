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

void print_inputs(neuron_t *net) {
    printf("Net: 0x%X, Afte inputs: ", net);
    for(uint32_t i=0; i<4; i++) {
        printf("%lf, ", net[i].output);
    }
    printf("\n");
}

double get_output(network_t *net, uint32_t num_inputs, double inputs[]) {
    // printf("Net: 0x%X, Init inputs: ", net->neurons);
    for(uint32_t i=0; i<num_inputs; i++) {
        // printf("%lf, ", inputs[i]);
        net->neurons[i].output = inputs[i];
    }
    // printf("\n");
    // print_inputs(net->neurons);
    uint32_t updated = 0;
    uint32_t counter = 0;
    // while(updated == 0 && counter++ < 100) {
        for(uint32_t i=0; i<(net->net_size); i++) {
            calc_output(&net->neurons[i], net->neurons);
        // }
        // for(uint32_t i=num_inputs; i<net->net_size;i++) {
            updated = update_output(&net->neurons[i]);
            // update_output(&net->neurons[i]);
        }
    // }
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
