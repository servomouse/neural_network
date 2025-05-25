#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

#define MUL 0
#define DIV 1

#define DIV_LOW_LIMIT (double)0.125

void neuron_poly_init(neuron_params_t * n_params, uint32_t num_inputs);
double neuron_poly_get_output(neuron_params_t *n_params, double *inputs);

// Mutations:
void neuron_poly_stash_state(neuron_params_t * n_params);
void neuron_poly_mutate(neuron_params_t * n_params);
void neuron_poly_rollback(neuron_params_t * n_params);

// Save-restore:
uint32_t neuron_poly_get_data_size(neuron_params_t *neuron);
compressed_neuron_t * neuron_poly_save(neuron_params_t * n_params); // Free the returned value after use
uint32_t neuron_poly_restore(neuron_params_t * n_params, compressed_neuron_t * n_data);
