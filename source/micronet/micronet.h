#pragma once

#include <stdint.h>
#include "neuron_types.h"

typedef struct {
    uint32_t idx;
    uint32_t num_inputs;
    neuron_type_t n_type;
    uint32_t indices[0];
} subneuron_description_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint32_t *neurons;  // subneuron_description_t is used here
    uint32_t num_outputs;
    uint32_t output_indices[];
} micronet_map_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint32_t num_outputs;
    uint32_t *output_indices;
    double *arr;
    double *outputs;
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
    micronet_map_t *map;
    feedback_item_t *feedback_arr;
} micro_network_t;

void micronet_init(micro_network_t * config, micronet_map_t *net_map, double **bckp_coeffs);
double *micronet_get_output(micro_network_t *config, double *inputs);
void micronet_save(micro_network_t * config, char *filename);
void micronet_restore(micro_network_t * config, char *filename);
void micronet_mutate(micro_network_t * config);
void micronet_rollback(micro_network_t * config);
void micronet_print_coeffs(micro_network_t * config);
void micronet_set_global_error(micro_network_t * config, double error);
void micronet_save_data(micro_network_t * config, char *filename, char *prefix, char *to_define);
void micronet_update_feedbacks(micro_network_t *config, micro_network_t *f_net);
void micronet_clear_feedbacks(micro_network_t *config);
