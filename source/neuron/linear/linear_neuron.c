#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "linear_neuron.h"

void neuron_linear_init(neuron_params_t * n_params, uint32_t num_inputs) {
    n_params->n_type = NLinear;
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = num_inputs+1;    // +1 for BIAS
    n_params->mutation_step = 0.1;
    n_params->is_mutable = 1;

    n_params->inputs = alloc_memory(n_params->inputs, n_params->num_inputs, sizeof(double));
    n_params->indices = alloc_memory(n_params->indices, n_params->num_inputs, sizeof(uint32_t));

    n_params->coeffs      = alloc_memory(n_params->coeffs, n_params->num_coeffs, sizeof(double));
    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(double));
    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(double));
    double *coeffs = (double*)n_params->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i] = random_double(-0.1, 0.1);
    }
}

double neuron_linear_get_output(neuron_params_t *n_params, double *inputs) {
    double *coeffs = (double*)n_params->coeffs;
    double output = coeffs[n_params->num_inputs];     // BIAS
    for(size_t i=0; i<n_params->num_inputs; i++) {
        uint32_t idx = n_params->indices[i];
        output += inputs[idx] * coeffs[i];
    }
    return output;
}
