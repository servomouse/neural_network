#pragma once

#include <stdint.h>

typedef struct {
    uint32_t idx;
    uint32_t num_inputs;
    uint32_t indices[8];
} subneuron_description_t;

typedef struct {
    uint8_t num_inputs;
    uint8_t num_neurons;
    uint8_t net_size;
    uint8_t output_idx;
    subneuron_description_t neurons[11];
} micronet_map_t;

typedef struct {
    uint32_t idx;
    uint32_t num_inputs;    // Also used as a size of the indices array
    uint32_t output_idx;    // Map this neuron to the outputs array with output_idx index, set to 0xFFFFFFFF if the neuron is not output
    uint32_t indices[];     // Which neurons are used as inputs for this neuron
} neuron_desc_t;

#define NOT_OUTPUT 0xFFFFFFFF

typedef struct {
    uint32_t counter;
    double value;
} feedback_item_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t net_size;      // Total net size, including inputs
    uint32_t num_outputs;
    uint32_t neurons[];    // neuron_desc_t is used here
} network_map_t;

typedef struct {
    double *inputs;
    uint32_t *indices;
    double *coeffs;
    double *backup_coeffs;
    double *last_vector;
    double *rand_vector;
    double *part_values;
    double *coeff_feedback;
    feedback_item_t **inputs_feedback;
    // micro_network_t *micro_net;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    uint32_t feedback_error_count;
    double *global_errors;    // Error of the entire network
    uint32_t last_idx;
    double last_value;
    double *outputs;
    uint32_t num_outputs;
    int32_t output_idx;     // Index in the output array where this neuron is mapped. Set to -1 if not mapped
    uint32_t num_feedbacks_received;
    uint32_t output_counter;
    uint32_t dataset_size;
    uint32_t mutated;
    uint32_t bad_mutations_counter;
    double mutation_step;
} neuron_params_t;

// typedef struct {
//     double *inputs;
//     uint32_t *indices;
//     struct {
//         double offset;
//         double amplitude;
//     } *coeffs;
//     // micro_network_t *micro_net;
//     uint32_t num_coeffs;
//     uint32_t num_inputs;
//     double feedback_error;  // Error calculated by neurons connected to the output
//     uint32_t feedback_error_count;
//     double global_error;    // Error of the entire network
//     uint32_t last_idx;
//     struct {
//         double offset;
//         double amplitude;
//     } last_value;
// } neuron_params_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint8_t output_idx;
    double *arr;
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
    micronet_map_t *map;
} micro_network_t;

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    uint32_t num_outputs;
    double *arr;
    uint32_t *output_indices;
    double *outputs;
    feedback_item_t **local_errors; // idx0 is neuron index, idx1 is sample index
    neuron_params_t *neurons;
    uint32_t mutated_neuron_idx;
    uint32_t last_mutated_micronet;
    micro_network_t *coeffs_micronet;
    micro_network_t *feedback_micronet;
    network_map_t *map;
    uint32_t dataset_size;
} network_t;
