#pragma once

#include <stdint.h>
#include "network.h"

uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons);
void network_init(micro_network_t * config, network_map_t *net_map);
double *network_get_output(micro_network_t *config, double *inputs);
void network_print_coeffs(micro_network_t * config);
