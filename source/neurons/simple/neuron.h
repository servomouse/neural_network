#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs);
double neuron_get_output(neuron_params_t * n_params, double *inputs);
void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);

void neuron_set_coeffs(neuron_params_t * n_params, double *coeffs);
void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, double value);
double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx);
uint32_t neuron_get_num_coeffs(neuron_params_t * n_params);

int neuron_get_coeffs_as_string(neuron_params_t *n_params, char *buffer, uint32_t buffer_size);

void neuron_stash_state(neuron_params_t * n_params);
void neuron_mutate(neuron_params_t * n_params);
void neuron_rollback(neuron_params_t * n_params);

uint32_t neuron_get_num_coeffs(neuron_params_t * n_params);
void neuron_print_coeffs(neuron_params_t * n_params);
