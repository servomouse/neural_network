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

// May be only single output
void micronet_init(micro_network_t * config, micronet_map_t *net_map) {
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->num_neurons;
    config->net_size = net_map->net_size;
    config->output_idx = net_map->output_idx;
    config->mutated_neuron_idx = 0;
    printf("Creating micro network with:\n");
    printf("\t%d inputs;\n", config->num_inputs);
    printf("\t%d neurons;\n", config->num_neurons);
    printf("\ttotal size is %d\n", config->net_size);

    config->arr = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    for(size_t i=0; i<config->num_neurons; i++) {
        uint8_t idx = net_map->neurons[i].idx - config->num_inputs;
        uint8_t num_inputs = net_map->neurons[i].num_inputs;
        neuron_init(&config->neurons[idx], num_inputs, 1);
        for(size_t j=0; j<net_map->neurons[i].num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, net_map->neurons[i].indices[j]);
            if(j >= config->num_inputs) {
                uint32_t temp_idx = j-config->num_inputs;
                neuron_set_num_outputs(&config->neurons[temp_idx], neuron_get_num_outputs(&config->neurons[temp_idx]) + 1);
            }
        }
    }
}

double micronet_get_output(micro_network_t * config, double *inputs) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_reset_output_counter(&config->neurons[i]);
    }
    // printf("Micronet array: ");
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr, 0);
        }
        // printf("%f, ", config->arr[i]);
    }
    // printf("\n");
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
    // config->mutated_neuron_idx = random_int(0, config->num_neurons);
    config->mutated_neuron_idx++;
    if(config->mutated_neuron_idx == config->num_neurons) {
        config->mutated_neuron_idx = 0;
    }
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_rollback(micro_network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_print_coeffs(micro_network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
