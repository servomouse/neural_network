#include <stdlib.h>
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
    .num_inputs = 4,
    .num_neurons = 1,
    .net_size = 5,
    .neurons = {
        {.idx = 4, .num_inputs = 4, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
    .output_idx = 4
};

micronet_map_t coeffs_micronet_map = {
    .num_inputs = 4,
    .num_neurons = 1,
    .net_size = 5,
    .neurons = {
        {.idx = 4, .num_inputs = 4, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
    .output_idx = 4
};

micro_network_t coeffs_micronet;
micro_network_t feedback_micronet;

void network_init(network_t * config, network_map_t *net_map) {
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->net_size - net_map->num_inputs;
    config->net_size = net_map->net_size;
    config->num_outputs = net_map->num_outputs;
    printf("Creating network with %d inputs and %d neurons, total size is %d\n", config->num_inputs, 
                                                                                 config->num_neurons,
                                                                                 config->net_size);
    config->arr = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    
    for(size_t i=0; i<config->num_neurons; i++) {
        uint8_t idx = net_map->neurons[i].idx - config->num_inputs;
        uint8_t num_inputs = net_map->neurons[i].num_inputs;
        neuron_init(&config->neurons[idx], num_inputs);
        for(size_t j=0; j<net_map->neurons[i].num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, net_map->neurons[i].indices[j]);
            if(j >= config->num_inputs) {
                uint32_t temp_idx = j-config->num_inputs;
                neuron_set_num_outputs(&config->neurons[temp_idx], neuron_get_num_outputs(&config->neurons[temp_idx]) + 1);
            }
        }
    }

    config->output_indices = calloc(config->num_outputs, sizeof(uint32_t));
    config->outputs = calloc(config->num_outputs, sizeof(double));
    for(size_t i=0; i<config->num_outputs; i++) {
        config->output_indices[i] = config->output_indices[i];
        config->outputs[i] = 0;
    }
    // Init micronets:
    micronet_init(&feedback_micronet, &feedback_micronet_map);
    micronet_init(&coeffs_micronet, &coeffs_micronet_map);
}

double* network_get_outputs(micro_network_t * config, double *inputs) {
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr);
        }
    }
    return config->arr[config->output_idx];
}

void micronet_set_global_error(micro_network_t *config, double error) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_set_global_error(&config->neurons[i-config->num_inputs], error);
    }
}

void micronet_save(micro_network_t * config, char *filename) {
    char *arr_fname = concat_strings("arr_", filename);
    store_data(&config, sizeof(config), filename);
    store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void micronet_restore(micro_network_t * config, char *filename) {
    if(!config->neurons) {
        printf("Call init() before calling restore_state()!");
        exit(1);
    }
    char *arr_fname = concat_strings("arr_", filename);
    restore_data(&config, sizeof(config), filename);
    restore_data(config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void micronet_mutate(micro_network_t * config) {
    config->mutated_neuron_idx = random_int(0, config->num_neurons);
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_rollback(micro_network_t * config) {
    neuron_restore(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_print_coeffs(micro_network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
