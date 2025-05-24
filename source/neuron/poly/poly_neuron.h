#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

void neuron_poly_init(neuron_params_t * n_params, uint32_t num_inputs);
double neuron_poly_get_output(neuron_params_t *n_params, double *inputs);