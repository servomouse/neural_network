#pragma once

#include <stdint.h>
#include "neuron_types.h"

void network_init(network_t * config, network_map_t *net_map);
double *network_get_outputs(network_t * config, double *inputs);
void network_save_to_file(network_t * config, char *filename);
void network_restore_from_file(network_t * config, char *filename);
void network_mutate(network_t * config);
void network_rollback(network_t * config);
void network_print_coeffs(network_t * config);
void network_set_global_error(network_t * config, double error);
void network_backup(network_t * config);
void network_restore(network_t * config);
