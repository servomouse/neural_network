#pragma once

#include <stdint.h>
#include "neuron_types.h"

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint32_t num_outputs;
    double *arr;
    uint32_t *output_indices;
    double *outputs;
    complex_value_t *feedback;
    complex_item_t *feedback_errors;
    complex_item_t *feedback_activations;
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
    uint32_t last_mutated_micronet;
    micro_network_t *coeffs_micronet;
    micro_network_t *feedback_micronet;
    network_map_t *map;
    uint32_t dataset_size;
} network_t;

void network_init(network_t * config, network_map_t *net_map, micro_network_t *c_micronet, micro_network_t *f_micronet);
double *network_get_outputs(network_t * config, double *inputs, uint32_t to_print);
// void network_set_local_errors(network_t * config, double *errors, uint32_t offset);
void network_backup(network_t * config);
void network_check_backup(network_t * config);
void network_restore(network_t * config);

void network_reset_activations(network_t * config);
void network_set_global_error(network_t * config, double error);
void network_set_output_errors(network_t * config, double *errors);
void network_generate_feedbacks(network_t * config);
void network_update_weights(network_t * config);

void network_stash_neurons(network_t * config);
// void network_update_neurons(network_t * config);
void network_rollback_neurons(network_t * config);

void network_reset_counters(network_t *config);

void network_mutate(network_t * config);
void network_rollback(network_t * config);
void network_mutate_micronet(network_t * config);
void network_rollback_micronet(network_t * config);

void network_save_data(network_t * config, char *filename);
void network_restore_data(network_t * config, char *filename);

void network_print_coeffs(network_t * config);


/*
    network_mutate_micronet();
    network_stash_neurons();
    for(int i=0; i<10; i++)
        network_update_neurons();
    [measure_error]
    network_rollback_neurons();
    optional: network_rollback_micronet();
*/
