#include <stdlib.h>
#include <string.h>
#include "network_common.h"
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

// Frees the prt if it is not NULL, then callocs the necessary amount of memory
// and checks if allocated successfully
static void * allocate(void *ptr, uint32_t num_elements, uint32_t sizeof_element) {
    free_if_needed(ptr);
    void * new_ptr = calloc(num_elements, sizeof_element);
    if(new_ptr == NULL) {
        printf("Error: failed to allocate memory!");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void network_init_micronet(network_t *net, network_map_t *net_map) {
    net_config_t unet_config = {
        .net_map = net_map,
        .c_linear_micronet = NULL,
        .c_poly_micronet = NULL,
        .f_micronet = NULL
    };
    network_init(net, &unet_config, 1);
}

static network_map_t * network_copy_map(network_map_t *src) {
    uint32_t num_outputs = src->num_outputs;
    network_map_t *dst = calloc(1, sizeof(network_map_t) + (sizeof(uint32_t) * num_outputs));
    dst->net_size = src->net_size;
    dst->num_inputs = src->num_inputs;
    dst->num_neurons = src->num_neurons;
    dst->num_outputs = num_outputs;
    for(uint32_t i=0; i<num_outputs; i++) {
        dst->output_indices[i] = src->output_indices[i];
    }
    uint32_t n_field_size = get_neurons_field_size(src->neurons, src->num_neurons);
    free_if_needed(dst->neurons);
    dst->neurons = calloc(n_field_size, 1);
    memcpy(dst->neurons, src->neurons, n_field_size);

    dst->neurons = &src->neurons[0];
    return dst;
}

void network_init(network_t *net, net_config_t *net_conf, uint8_t is_micronet) {
    free_if_needed(net->map);
    net->map = network_copy_map(net_conf->net_map);
    net->num_inputs = net_conf->net_map->num_inputs;
    net->num_neurons = net_conf->net_map->num_neurons;
    net->net_size = net_conf->net_map->net_size;
    net->num_outputs = net_conf->net_map->num_outputs;
    net->mutated_neuron_idx = 0;

    net->output_indices = allocate(net->output_indices, net->num_outputs, sizeof(uint32_t));
    net->outputs = allocate(net->outputs, net->num_outputs, sizeof(double));
    for(uint32_t i=0; i<net->num_outputs; i++) {
        net->output_indices[i] = net_conf->net_map->output_indices[i];
    }

    net->arr = allocate(net->arr, net->net_size, sizeof(double));
    // net->feedback_arr = allocate(net->feedback_arr, net->net_size, sizeof(double));
    net->neurons = allocate(net->neurons, net->num_neurons, sizeof(neuron_params_t));

    uint32_t offset = 0;
    for(uint32_t i=0; i<net->num_neurons; i++) {
        subneuron_description_t *neuron = (subneuron_description_t *)&net_conf->net_map->neurons[offset];
        uint32_t idx        = neuron->idx - net->num_inputs;
        uint32_t num_inputs = neuron->num_inputs;
        uint32_t n_type = neuron->n_type;
        neuron_init(&net->neurons[idx], n_type, num_inputs);
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&net->neurons[idx], j, neuron->indices[j]);
        }
        offset += 3 + num_inputs;
    }
    net->is_micronet = is_micronet;
    if(is_micronet == 0) {
        net->c_linear_micronet = net_conf->c_linear_micronet;
        net->c_poly_micronet = net_conf->c_poly_micronet;
        net->f_micronet = net_conf->f_micronet;
    }
}

double *network_get_output(network_t * config, double *inputs) {
    // for(int i=0; i<config->num_neurons; i++) {
    //     neuron_reset_output_counter(&config->neurons[i]);
    // }
    // printf("network array: ");
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

void network_print_coeffs(network_t * config) {
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
