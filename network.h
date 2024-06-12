#pragma once

#include "stdint.h"
#include "string.h"
#include "neuron.h"

typedef struct {
    neuron_t *neurons;
    uint32_t net_size;
    uint32_t num_inputs;
    uint32_t num_outputs;
    uint32_t last_mutant_idx;
} network_t;

network_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs);
double get_output(network_t *net, uint32_t num_inputs, double inputs[]);
void save_network(network_t *net, char *filename);
void restore_network(char *filename);
void mutate(network_t *net);
void repair(network_t *net);
