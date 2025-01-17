#include <stdlib.h>
#include <string.h>
#include "micro_net.h"
#include "network_new.h"
#include "neuron.h" // includes neuron_types.h
#include "utils.h"
#include <time.h>

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

micronet_map_t feedback_micronet_map = {
    .num_inputs = 5,
    .num_neurons = 11,
    .net_size = 16,
    .neurons = {
        {.idx = 5, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 6, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 7, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 8, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 9, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 10, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
        {.idx = 11, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
        {.idx = 12, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
        {.idx = 13, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
        {.idx = 14, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
        {.idx = 15, .num_inputs = 5, .indices = {10, 11, 12, 13, 14, 0, 0, 0}},
    },
    .output_idx = 15
};

micronet_map_t coeffs_micronet_map = {
    .num_inputs = 5,
    .num_neurons = 6,
    .net_size = 11,
    .neurons = {
        {.idx = 5, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 6, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 7, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 8, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 9, .num_inputs = 5, .indices = {0, 1, 2, 3, 4, 0, 0, 0}},
        {.idx = 10, .num_inputs = 5, .indices = {5, 6, 7, 8, 9, 0, 0, 0}},
    },
    .output_idx = 10
};

neuron_params_t *neurons_backup;
double *arr_backup;

void network_init(network_t * config, network_map_t *net_map, uint32_t dataset_size) {
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->net_size - net_map->num_inputs;
    config->net_size = net_map->net_size;
    config->num_outputs = net_map->num_outputs;
    printf("Creating network with:\n");
    printf("\t%d inputs;\n", config->num_inputs);
    printf("\t%d output(s);\n", config->num_outputs);
    printf("\t%d neurons;\n", config->num_neurons);
    printf("\ttotal size is %d\n", config->net_size);
    
    config->arr = calloc(config->net_size, sizeof(double));
    arr_backup = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    neurons_backup = calloc(config->num_neurons, sizeof(neuron_params_t));
    config->output_indices = calloc(config->num_outputs, sizeof(uint32_t));
    config->outputs = calloc(config->num_outputs, sizeof(double));
    
    uint32_t offset = 0;
    for(size_t i=0; i<config->num_neurons; i++) {
        neuron_desc_t *neuron = (neuron_desc_t *)&net_map->neurons[offset];
        uint8_t idx = neuron->idx - config->num_inputs;
        uint8_t num_inputs = neuron->num_inputs;
        neuron_init(&config->neurons[idx], num_inputs, dataset_size);
        neuron_set_output_idx(&config->neurons[idx], neuron->output_idx);
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, neuron->indices[j]);
            if(j >= config->num_inputs) {
                uint32_t temp_idx = j-config->num_inputs;
                neuron_set_num_outputs(&config->neurons[temp_idx], neuron_get_num_outputs(&config->neurons[temp_idx]) + 1);
            }
        }
        if(neuron->output_idx != NOT_OUTPUT) {
            config->output_indices[neuron->output_idx] = config->num_inputs + i;
        }
        offset += num_inputs + 3;
    }
    // Init micronets:
    micronet_init(&config->feedback_micronet, &feedback_micronet_map);
    micronet_init(&config->coeffs_micronet, &coeffs_micronet_map);
}

void network_backup(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_backup(&config->neurons[i]);
    }
    memcpy(neurons_backup, config->neurons, sizeof(neuron_params_t) * config->num_neurons);
    memcpy(arr_backup, config->arr, sizeof(double) * config->net_size);
}

void network_restore(network_t * config) {
    memcpy(config->neurons, neurons_backup, sizeof(neuron_params_t) * config->num_neurons);
    memcpy(config->arr, arr_backup, sizeof(double) * config->net_size);
    for(int i=0; i<config->num_neurons; i++) {
        neuron_restore(&config->neurons[i]);
    }
}

double* network_get_outputs(network_t * config, double *inputs, uint32_t to_print) {
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr, to_print);
        }
    }
    for(size_t i=0; i<config->num_outputs; i++) {
        config->outputs[i] = config->arr[config->output_indices[i]];
    }
    return config->outputs;
}

void network_reset_counters(network_t *config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_reset_output_counter(&config->neurons[i]);
    }
}

void network_set_global_error(network_t *config, double error) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_set_global_error(&config->neurons[i], error);
    }
}

void network_save_to_file(network_t * config, char *filename) {
    char *arr_fname = concat_strings("arr_", filename);
    store_data(&config, sizeof(config), filename);
    store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void network_restore_from_file(network_t * config, char *filename) {
    if(!config->neurons) {
        printf("Call init() before calling restore_state()!");
        exit(1);
    }
    char *arr_fname = concat_strings("arr_", filename);
    restore_data(&config, sizeof(config), filename);
    restore_data(config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void network_update(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_update_coeffs(&config->neurons[i], &config->coeffs_micronet);
    }
}

void network_mutate(network_t * config) {
    config->mutated_neuron_idx = random_int(0, config->num_neurons);
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void network_rollback(network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}

void network_mutate_micronet(network_t * config) {
    config->last_mutated_micronet = random_bit();
    if(config->last_mutated_micronet) {
        micronet_mutate(&config->feedback_micronet);
    } else {
        micronet_mutate(&config->coeffs_micronet);
    }
}

void network_rollback_micronet(network_t * config) {
    if(config->last_mutated_micronet) {
        micronet_rollback(&config->feedback_micronet);
    } else {
        micronet_rollback(&config->coeffs_micronet);
    }
}

void network_print_coeffs(network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
