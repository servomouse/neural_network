#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "poly_neuron.h"

void neuron_poly_init(neuron_params_t * n_params, uint32_t num_inputs) {
    n_params->n_type = NPoly;
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = 1 << num_inputs;
    n_params->mutation_step = 0.1;
    n_params->is_mutable = 1;

    n_params->inputs = alloc_memory(n_params->inputs, n_params->num_inputs, sizeof(double));
    n_params->indices = alloc_memory(n_params->indices, n_params->num_inputs, sizeof(uint32_t));

    n_params->coeffs      = alloc_memory(n_params->coeffs,      n_params->num_coeffs, sizeof(complex_coeff_t));
    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(complex_coeff_t));
    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(complex_coeff_t));

    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i].val[MUL] = random_double(-0.1, 0.1);
        coeffs[i].val[DIV] = random_double(DIV_LOW_LIMIT, 1.0);
    }
}

double neuron_poly_get_output(neuron_params_t *n_params, double *inputs) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    double output = coeffs[0].val[MUL];     // BIAS
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                uint32_t idx = n_params->indices[j];
                temp *= inputs[idx];
            }
        }
        output += (temp * coeffs[i].val[MUL]) / coeffs[i].val[DIV];
    }
    return output;
}

void neuron_poly_set_coeffs(neuron_params_t * n_params, void *new_coeffs) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    complex_coeff_t *new_values = (complex_coeff_t*)new_coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i].val[MUL] = new_values[i].val[MUL];
        coeffs[i].val[DIV] = new_values[i].val[DIV];
    }
}

void neuron_poly_set_coeff(neuron_params_t * n_params, uint32_t idx, void *new_coeff) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    complex_coeff_t *new_value = (complex_coeff_t*)new_coeff;
    if(idx < n_params->num_coeffs) {
        coeffs[idx].val[MUL] = new_value[0].val[MUL];
        coeffs[idx].val[DIV] = new_value[0].val[DIV];
    }
}

double neuron_poly_get_coeff(neuron_params_t * n_params, uint32_t idx) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    return coeffs[idx].val[MUL];
}

void neuron_poly_print_coeffs(neuron_params_t * n_params) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("[%f, %f], ", coeffs[i].val[MUL], coeffs[i].val[DIV]);
    }
    printf("\n");
}
