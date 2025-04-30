#pragma once

#include <stdint.h>
#include "network.h"

void network_init_micronet(network_t *net, network_map_t *net_map);
void network_init(network_t * config, net_config_t *net_conf, uint8_t is_micronet);
double *network_get_output(network_t *config, double *inputs);
void network_clear_outputs(network_t * config);

void network_print_coeffs(network_t * config);
uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons);
