#pragma once

#include <stdint.h>
#include "neuron_types.h"

void micronet_init(micro_network_t * config, micronet_map_t *net_map);
double micronet_get_output(micro_network_t * config, double *inputs);
void micronet_save(micro_network_t * config, char *filename);
void micronet_restore(micro_network_t * config, char *filename);
void micronet_mutate(micro_network_t * config);
void micronet_rollback(micro_network_t * config);
void micronet_print_coeffs(micro_network_t * config);
void micronet_set_global_error(micro_network_t * config, double *inputs);
