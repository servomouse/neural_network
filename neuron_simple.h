#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

void mini_neuron_init(simple_neuron_params_t * n_params, uint32_t num_inputs);
double mini_neuron_get_output(simple_neuron_params_t * n_params, double *inputs);
void mini_neuron_set_input_idx(simple_neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);
void mini_neuron_set_coeffs(simple_neuron_params_t * n_params, double *coeffs);
int mini_neuron_get_coeffs_as_string(simple_neuron_params_t *n_params, char *buffer, uint32_t buffer_size);

void mini_neuron_stash_state(simple_neuron_params_t * n_params);
void mini_neuron_mutate(simple_neuron_params_t * n_params);
void mini_neuron_rollback(simple_neuron_params_t * n_params);

uint32_t mini_neuron_get_num_coeffs(simple_neuron_params_t * n_params);
void mini_neuron_print_coeffs(simple_neuron_params_t * n_params);
