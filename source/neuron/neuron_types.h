#pragma once

#include <stdint.h>

#define FEEDBACK_UNET_SIZE 10

typedef struct {
    uint32_t idx;
    uint32_t num_inputs;    // Also used as a size of the indices array
    uint32_t output_idx;    // Map this neuron to the outputs array with output_idx index, set to 0xFFFFFFFF if the neuron is not output
    uint32_t indices[];     // Which neurons are used as inputs for this neuron
} neuron_desc_t;

#define NOT_OUTPUT 0xFFFFFFFF

typedef struct {
    uint32_t counter;
    double min_value;
    double max_value;
    double tot_value;
    double tot_value_mod;
} complex_item_t;

typedef struct {
    uint32_t counter;
    double value;
} complex_value_t;

// typedef struct {
//     uint32_t num_inputs;
//     uint32_t net_size;      // Total net size, including inputs
//     uint32_t num_outputs;
//     uint32_t neurons[];    // neuron_desc_t is used here
// } network_map_t;

typedef enum uint32_t {
    NLinear = 0,
    NPoly,
    NSmart
} neuron_type_t;

typedef struct {
    uint32_t size;  // Size of the resulting structure, bytes
    neuron_type_t n_type;
    uint32_t num_inputs;
    uint32_t num_coeffs;
    double coeffs[0];
} compressed_neuron_t;

typedef struct {
    double feedback_arr[FEEDBACK_UNET_SIZE];
    // Feedback micronet outputs:
    double error;
} feedback_item_t;

typedef struct {
    neuron_type_t n_type;
    double *inputs;
    // double *direct_inputs;
    // double *micronet_msg;
    // double *input_feedbacks;
    // uint32_t inputs_feedback_counter;
    uint32_t *indices;
    double *coeffs;
    double *c_deltas;
    double *c_net_stash;        // Used for linear neurons
    double *c_net_poly_stash;   // Used for poly neurons
    // double *f_net_stash;
    double *backup_coeffs;
    double *last_vector;
    double *rand_vector;
    // double *coeff_feedback;
    // double *part_values;
    // double *part_sums;
    // double *part_feedbacks;
    double output;
    // complex_value_t *inputs_feedback;    // Array for temporary values
    // double *feedback_micronet_stash;
    // double *coeffs_micronet_stash;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    // double feedback_error;  // Error calculated by neurons connected to the output
    // uint32_t feedback_error_count;
    feedback_item_t feedback;
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    double last_value;
    uint32_t num_outputs;
    int32_t output_idx;     // Index in the output array where this neuron is mapped. Set to -1 if not mapped
    // uint32_t num_feedbacks_received;
    uint32_t output_counter;
    uint32_t dataset_size;
    uint32_t mutated;
    uint32_t bad_mutations_counter;
    double mutation_step;
} neuron_params_t;

// typedef struct {
//     uint32_t counter;
//     double value;
//     double stash;
// } feedback_item_t;
