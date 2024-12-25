#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    double *inputs;
    uint32_t *indices;
    double *coeffs;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    uint32_t feedback_error_count;
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    double last_value;
} neuron_params_t;

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs);
void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);
void neuron_save_state(neuron_params_t * n_params, char *filename);
void neuron_restore_state(neuron_params_t * n_params, char *filename);
double neuron_get_output(neuron_params_t * n_params, double *inputs);
void neuron_set_feedback_error(neuron_params_t * n_params, double error);
void neuron_set_global_error(neuron_params_t * n_params, double error);
void neuron_mutate(neuron_params_t * n_params);
void neuron_restore(neuron_params_t * n_params);
void neuron_print_coeffs(neuron_params_t * n_params);
