#include <stdlib.h>
#include "micro_net.h"
#include "neuron.h"
#include "utils.h"
#include <time.h>

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

void micronet_init(micro_network_t * config, micronet_map_t *net_map) {
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->num_neurons;
    config->net_size = net_map->net_size;
    printf("Creating network with %d inputs and %d neurons, total size is %d\n", config->num_inputs, 
                                                                                 config->num_neurons,
                                                                                 config->net_size);
    config->arr = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    for(size_t i=0; i<config->num_neurons; i++) {
        uint8_t idx = net_map->neurons[i].idx;
        uint8_t num_inputs = net_map->neurons[i].num_inputs;
        neuron_init(&(config->neurons[idx]), num_inputs);
    }
}

double micronet_get_output(micro_network_t * config, double *inputs) {
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i], config->arr);
        }
    }
    return config->arr[config->output_idx];
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
