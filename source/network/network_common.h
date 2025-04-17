#pragma once

#include <stdint.h>
#include "network.h"

uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons);
void network_init(network_t * config, net_config_t *net_conf);
double *network_get_output(network_t *config, double *inputs);
void network_print_coeffs(network_t * config);
