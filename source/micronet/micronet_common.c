#include <stdlib.h>
#include <string.h>
#include "micronet_common.h"
#include "neuron.h"
#include "neuron_types.h"
#include "utils.h"
#include <time.h>

uint32_t get_neurons_field_size(uint32_t *neurons, uint32_t num_neurons) {
    uint32_t offset = 0;
    uint32_t field_size = 0;
    for(uint32_t i=0; i<num_neurons; i++) {
        subneuron_description_t *n = (subneuron_description_t *)&neurons[offset];
        field_size += 3 + n->num_inputs;
        offset += 3 + n->num_inputs;
    }
    return field_size * sizeof(uint32_t);
}

void free_if_needed(void *ptr) {
    if(ptr != NULL) {
        free(ptr);
    }
}

void micronet_init(micro_network_t * config, micronet_map_t *net_map) {
    free_if_needed(config->map);
    config->map = calloc(1, sizeof(micronet_map_t) + (sizeof(uint32_t) * config->num_outputs));
    config->map->net_size = net_map->net_size;
    config->map->num_inputs = net_map->num_inputs;
    config->map->num_neurons = net_map->num_neurons;
    config->map->num_outputs = net_map->num_outputs;
    for(uint32_t i=0; i<net_map->num_outputs; i++) {
        config->map->output_indices[i] = net_map->output_indices[i];
    }
    uint32_t n_field_size = get_neurons_field_size(net_map->neurons, net_map->num_neurons);
    free_if_needed(config->map->neurons);
    config->map->neurons = calloc(n_field_size, 1);
    memcpy(config->map->neurons, net_map->neurons, n_field_size);

    config->map->neurons = &net_map->neurons[0];
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->num_neurons;
    config->net_size = net_map->net_size;
    config->num_outputs = net_map->num_outputs;
    config->mutated_neuron_idx = 0;
    // printf("Creating micro network with:\n");
    // printf("\t%d inputs;\n", config->num_inputs);
    // printf("\t%d neurons;\n", config->num_neurons);
    // printf("\tnum outputs: %d;\n", config->num_outputs);

    free_if_needed(config->output_indices);
    config->output_indices = calloc(config->num_outputs, sizeof(uint32_t));
    free_if_needed(config->outputs);
    config->outputs = calloc(config->num_outputs, sizeof(double));
    // printf("\toutput indices: [");
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->output_indices[i] = net_map->output_indices[i];
        // if(i == config->num_outputs-1)
        //     printf("%d]\n", net_map->output_indices[i]);
        // else
        //     printf("%d, ", net_map->output_indices[i]);
    }
    // printf("\ttotal micronet size is %d\n", config->net_size);

    free_if_needed(config->arr);
    config->arr          = calloc(config->net_size, sizeof(double));

    free_if_needed(config->feedback_arr);
    config->feedback_arr = calloc(config->net_size, sizeof(feedback_item_t));

    free_if_needed(config->neurons);
    config->neurons      = calloc(config->num_neurons, sizeof(neuron_params_t));

    uint32_t offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        subneuron_description_t *neuron = (subneuron_description_t *)&net_map->neurons[offset];
        uint32_t idx        = neuron->idx - config->num_inputs;
        uint32_t num_inputs = neuron->num_inputs;
        uint32_t n_type = neuron->n_type;
        // printf("Neuron %d: idx = %d, num_inputs = %d, offset = %d\n", i, idx, num_inputs, offset);
        neuron_init(&config->neurons[idx], n_type, num_inputs);
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, neuron->indices[j]);
        }
        offset += 3 + num_inputs;
    }
}

double *micronet_get_output(micro_network_t * config, double *inputs) {
    // for(int i=0; i<config->num_neurons; i++) {
    //     neuron_reset_output_counter(&config->neurons[i]);
    // }
    // printf("Micronet array: ");
    for(uint32_t i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr);
        }
        // printf("%f, ", config->arr[i]);
    }
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->outputs[i] = config->arr[config->output_indices[i]];
    }
    if(config->outputs[0] != config->outputs[0]) {
        printf("Micornet error! array:\n");
        for(uint32_t i=0; i<config->net_size; i++) {
            printf("%f, ", config->arr[i]);
        }
        printf("\n");
        printf("Inputs:\n");
        for(uint32_t i=0; i<config->num_inputs; i++) {
            printf("%f, ", inputs[i]);
        }
        printf("\n");
        exit(0);
    }
    // printf("\n");
    return config->outputs;
}

void micronet_print_coeffs(micro_network_t * config) {
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
