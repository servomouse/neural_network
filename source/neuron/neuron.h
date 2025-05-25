#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

void neuron_init(neuron_params_t * n_params, neuron_type_t n_type, uint32_t num_inputs);
double neuron_get_output(neuron_params_t * n_params, double *inputs);


// Helper functions:
void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);
void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, void *new_coeff);
void neuron_set_coeffs(neuron_params_t * n_params, void *new_coeffs);
double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx);
void neuron_print_coeffs(neuron_params_t * n_params);


// Save/restore functionality:
uint32_t neuron_get_data_size(neuron_params_t *neuron);
compressed_neuron_t * neuron_save(neuron_params_t * n_params);
uint32_t neuron_restore(neuron_params_t * n_params, compressed_neuron_t * n_data);


// Mutations:
void neuron_stash_state(neuron_params_t * n_params);
void neuron_mutate(neuron_params_t * n_params);
void neuron_rollback(neuron_params_t * n_params);
