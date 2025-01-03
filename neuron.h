#pragma once
// python new_build.py controller
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

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
