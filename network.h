#pragma once

#include "stdint.h"
#include "string.h"
#include "neuron.h"

typedef struct {
    neuron_t * neurons;
    uint32_t net_size;
    uint32_t num_inputs;
    uint32_t num_outputs;
} network_t;

network_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs);
void save_network(neuron_t *net, char *filename);
void restore_network(char *filename);