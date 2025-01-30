#pragma once
// python new_build.py controller
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

void neuron_init(simple_neuron_params_t * n_params, uint32_t num_inputs, uint32_t dataset_size);
double neuron_get_output(simple_neuron_params_t * n_params, double *inputs);
void neuron_set_input_idx(simple_neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);
void neuron_set_coeffs(simple_neuron_params_t * n_params, double *coeffs);
int neuron_get_coeffs_as_string(simple_neuron_params_t *n_params, char *buffer, uint32_t buffer_size);

void neuron_stash_state(simple_neuron_params_t * n_params);
void neuron_mutate(simple_neuron_params_t * n_params);
void neuron_rollback(simple_neuron_params_t * n_params);

uint32_t neuron_get_num_coeffs(simple_neuron_params_t * n_params);
void neuron_print_coeffs(simple_neuron_params_t * n_params);
