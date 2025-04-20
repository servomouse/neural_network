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
} network_map_t;

typedef struct network_t {
    uint8_t is_micronet;
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint32_t num_outputs;
    uint32_t *output_indices;
    double *arr;
    double *outputs;
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
    network_map_t *map;
    // double *feedback_arr;
    struct network_t * c_linear_micronet;
    struct network_t * c_poly_micronet;
    struct network_t * f_micronet;
} network_t;

typedef struct {
    network_map_t *net_map;
    network_t *c_linear_micronet;
    network_t *c_poly_micronet;
    network_t *f_micronet;
} net_config_t;

#include "network_common.h"
#include "network_mutations.h"
#include "network_save_restore.h"
// #include "network_train.h"
// #include "neuron_iface.h"

// Network train:
void network_clear_feedbacks(network_t *config);
void network_set_global_error(network_t *config, double error, double *output_errors);
void network_update_feedbacks(network_t *config);
void network_update_coeffs(network_t *config);

// void network_init(micro_network_t * config, network_map_t *net_map);
// double *network_get_output(micro_network_t *config, double *inputs);
// void network_save(micro_network_t * config, char *filename);
// void network_restore(micro_network_t * config, char *filename);
// void network_mutate(micro_network_t * config);
// void network_rollback(micro_network_t * config);
// void network_print_coeffs(micro_network_t * config);
// void network_set_global_error(micro_network_t * config, double error, double *output_errors);
// void network_save_data(micro_network_t * config, char *filename, char *prefix, char *to_define);

// void network_update_coeffs(micro_network_t *config, micro_network_t *c_net);
// void network_update_feedbacks(micro_network_t *config, micro_network_t *f_net);
// void network_clear_feedbacks(micro_network_t *config);
// uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons);
