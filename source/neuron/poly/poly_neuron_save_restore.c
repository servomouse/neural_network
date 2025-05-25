#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "poly_neuron.h"

uint32_t neuron_poly_get_data_size(neuron_params_t *neuron) {
    uint32_t data_size = sizeof(compressed_neuron_t) + (sizeof(complex_coeff_t) * neuron->num_coeffs);
    return data_size;
}

// Free the returned value after use
compressed_neuron_t * neuron_poly_save(neuron_params_t * n_params) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    uint32_t data_size = sizeof(compressed_neuron_t) + (sizeof(complex_coeff_t) * n_params->num_coeffs);
    compressed_neuron_t *n_data = calloc(data_size, 1);
    n_data->size = data_size;
    n_data->n_type = n_params->n_type;
    n_data->num_inputs = n_params->num_inputs;
    n_data->num_coeffs = n_params->num_coeffs;
    complex_coeff_t *c_coeffs = (complex_coeff_t *)n_data->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        c_coeffs[i].val[MUL] = coeffs[i].val[MUL];
        c_coeffs[i].val[DIV] = coeffs[i].val[DIV];
    }
    return n_data;
}

uint32_t neuron_poly_restore(neuron_params_t * n_params, compressed_neuron_t * n_data) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    uint32_t ret_val = n_data->size;
    n_params->n_type = n_data->n_type;
    n_params->num_inputs = n_data->num_inputs;
    n_params->num_coeffs = n_data->num_coeffs;
    complex_coeff_t *c_coeffs = (complex_coeff_t *)n_data->coeffs;
    for(uint32_t i=0; i<n_data->num_coeffs; i++) {
        coeffs[i].val[MUL] = c_coeffs[i].val[MUL];
        coeffs[i].val[DIV] = c_coeffs[i].val[DIV];
    }
    return ret_val;
}