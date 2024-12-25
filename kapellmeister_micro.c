#include <stdlib.h>
#include "kapellmeister_micro.h"
#include "neuron.h"
#include "utils.h"
#include <time.h>

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    double *arr;
    neuron_params_t * neurons;
    uint32_t mutated_neuron_idx;
} micro_network_t;

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

typedef struct {
    uint32_t idx;
    uint8_t num_inputs;
    uint32_t indices[8];
} subneuron_description_t;

typedef struct {
    uint8_t num_inputs;
    uint8_t num_neurons;
    uint8_t net_size;
    subneuron_description_t neurons[7];
    uint8_t output_idx;
} micronet_map_t;

micronet_map_t micronet_map = {
    .num_inputs = 6,
    .num_neurons = 7,
    .net_size = 13,
    .neurons = {
        {.idx = 6, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
    .output_idx = 12
};

micro_network_t micro_net;

void init(uint32_t num_inputs, uint32_t net_size) {
    micro_net.num_inputs = num_inputs;
    micro_net.num_neurons = net_size - num_inputs;
    micro_net.net_size = net_size;
    printf("Creating network with %d inputs and %d neurons, total size is %d\n", micro_net.num_inputs, 
                                                                                 micro_net.num_neurons,
                                                                                 micro_net.net_size);
    micro_net.neurons = calloc(micro_net.net_size, sizeof(double));
    micro_net.neurons = calloc(micro_net.num_neurons, sizeof(neuron_params_t));
}

void init_neuron(uint32_t idx, uint32_t num_inputs) {
    neuron_init(&micro_net.neurons[idx], num_inputs);
}

DLL_PREFIX
void set_neuron_input_idx(uint32_t neuron_idx, uint32_t input_number, uint32_t input_idx) {
    neuron_set_input_idx(&micro_net.neurons[neuron_idx], input_number, input_idx);
}

DLL_PREFIX
void set_value(uint32_t idx, double value) {
    micro_net.arr[idx] = value;
}

DLL_PREFIX
double get_value(uint32_t idx) {
    return micro_net.arr[idx];
}

DLL_PREFIX
void tick_network(void) {
    for(int i=0; i<micro_net.num_neurons; i++) {
        micro_net.arr[micro_net.num_inputs+i] = neuron_get_output(&micro_net.neurons[i], micro_net.arr);
    }
}

void save_state(char *filename) {
    char *arr_fname = concat_strings("arr_", filename);
    store_data(&micro_net, sizeof(micro_net), filename);
    store_data(&micro_net.arr, micro_net.net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

DLL_PREFIX
void restore_state(char *filename) {
    if(!micro_net.neurons) {
        printf("Call init() before calling restore_state()!");
        exit(1);
    }
    char *arr_fname = concat_strings("arr_", filename);
    restore_data(&micro_net, sizeof(micro_net), filename);
    restore_data(micro_net.arr, micro_net.net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

DLL_PREFIX
void mutate(void) {
    micro_net.mutated_neuron_idx = random_int(0, micro_net.num_neurons);
    neuron_mutate(&micro_net.neurons[micro_net.mutated_neuron_idx]);
}

DLL_PREFIX
void restore(void) {
    neuron_restore(&micro_net.neurons[micro_net.mutated_neuron_idx]);
}
