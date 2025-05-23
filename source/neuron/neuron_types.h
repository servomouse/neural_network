#pragma once

#include <stdint.h>

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

typedef struct {
    double mul;
    double div;
} complex_coeff_t;

typedef enum uint32_t {
    NLinear = 0,    // Simple perceptron
    NPoly,          // Polynomial neuron
    Pattern,        // Pattern recognition
    Relay           // Simple relay neuron (output == single input)
} neuron_type_t;

typedef struct {
    uint32_t size;  // Size of the resulting structure, bytes
    neuron_type_t n_type;
    uint32_t num_inputs;
    uint32_t num_coeffs;
    double coeffs[0];
} compressed_neuron_t;

typedef struct {
    double *feedback_arr;
    double error;
} feedback_item_t;

typedef struct {
    neuron_type_t n_type;
    double *inputs;
    uint32_t *indices;
    double *coeffs;
    double *c_deltas;
    double *c_net_stash;
    double *backup_coeffs;
    double *last_vector;
    double *rand_vector;
    double output;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    feedback_item_t feedback;
    double global_error;    // Error of the entire network
    uint32_t mutated;
    uint32_t bad_mutations_counter;
    double mutation_step;
    uint8_t is_mutable;
} neuron_params_t;
