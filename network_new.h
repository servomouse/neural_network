#pragma once

#include <stdint.h>
#include "neuron_types.h"

void network_init(network_t * config, network_map_t *net_map, uint32_t dataset_size);
double *network_get_outputs(network_t * config, double *inputs, uint32_t to_print);
void network_set_global_error(network_t * config, double error);
void network_backup(network_t * config);
void network_restore(network_t * config);
void network_update(network_t * config);
void network_reset_counters(network_t *config);

void network_mutate(network_t * config);
void network_rollback(network_t * config);
void network_mutate_micronet(network_t * config);
void network_rollback_micronet(network_t * config);

void network_save_to_file(network_t * config, char *filename);
void network_restore_from_file(network_t * config, char *filename);

void network_print_coeffs(network_t * config);
