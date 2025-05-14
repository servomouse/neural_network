#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"

void neuron_linear_init(neuron_params_t * n_params, uint32_t num_inputs) {
    // printf("Creating neuron with %d inputs, %s\n", num_inputs, BCKP_DIR_PATH);
    n_params->n_type = NLinear;
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = num_inputs+1;    // +1 for BIAS
    n_params->mutation_step = 0.1;
    n_params->is_mutable = 1;

    n_params->inputs = alloc_memory(n_params->inputs, n_params->num_inputs, sizeof(double));
    n_params->indices = alloc_memory(n_params->indices, n_params->num_inputs, sizeof(uint32_t));

    n_params->coeffs = alloc_memory(n_params->coeffs, n_params->num_coeffs, sizeof(double));
    n_params->c_net_stash = alloc_memory(n_params->c_net_stash, n_params->num_coeffs, sizeof(double));
    n_params->c_deltas = alloc_memory(n_params->c_deltas, n_params->num_coeffs, sizeof(double));
    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(double));
    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(double));
    n_params->backup_coeffs = alloc_memory(n_params->backup_coeffs, n_params->num_coeffs, sizeof(double));
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = random_double(-0.1, 0.1);
        n_params->backup_coeffs[i] = n_params->coeffs[i];
    }

    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(double));
    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(double));
}

double neuron_linear_get_output(neuron_params_t *n_params, double *inputs) {
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[n_params->indices[i]];
    }
    double output = n_params->coeffs[n_params->num_inputs];         // BIAS
    for(size_t i=0; i<n_params->num_inputs; i++) {
        uint32_t idx = n_params->indices[i];
        output += inputs[idx] * n_params->coeffs[i];
    }
    return output;
}
