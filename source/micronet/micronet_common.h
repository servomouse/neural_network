#pragma once

#include <stdint.h>
#include "micronet.h"

uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons);
void micronet_init(micro_network_t * config, micronet_map_t *net_map);
double *micronet_get_output(micro_network_t *config, double *inputs);
void micronet_print_coeffs(micro_network_t * config);
