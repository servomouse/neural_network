#pragma once

#include <stdint.h>

typedef struct {
    uint32_t idx;
    uint8_t num_inputs;
    uint32_t indices[8];
} subneuron_description_t;

typedef struct {
    uint8_t num_inputs;
    uint8_t num_neurons;
    uint8_t net_size;
    struct {
        uint32_t idx;
        uint8_t num_inputs;
        uint32_t indices[8];
    } neurons[7];
    uint8_t output_idx;
} micronet_map_t;

// typedef struct neuron_params_t neuron_params_t;

// typedef struct {
//     double *inputs;
//     uint32_t *indices;
//     double *coeffs;
//     // micro_network_t *micro_net;
//     uint32_t num_coeffs;
//     uint32_t num_inputs;
//     double feedback_error;  // Error calculated by neurons connected to the output
//     uint32_t feedback_error_count;
//     double global_error;    // Error of the entire network
//     uint32_t last_idx;
//     double last_value;
// } neuron_params_t;

typedef struct {
    double *inputs;
    uint32_t *indices;
    struct {
        double offset;
        double amplitude;
    } *coeffs;
    // micro_network_t *micro_net;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    uint32_t feedback_error_count;
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    struct {
        double offset;
        double amplitude;
    } last_value;
} neuron_params_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint8_t output_idx;
    double *arr;
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
} micro_network_t;
