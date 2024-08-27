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

typedef struct {
	uint32_t num_inputs;
	uint32_t num_outputs;
	uint32_t num_neurons;   // Num neurons in hidden layers
} net_config_t;

network_t* create_network(net_config_t *net_conf);
double* get_output(network_t *net, double inputs[]);
void mutate(network_t *net);
void repair(network_t *net);
void print_results(network_t *net);
