#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "linear_neuron.h"

uint32_t neuron_linear_get_data_size(neuron_params_t *neuron) {
    uint32_t data_size = sizeof(compressed_neuron_t) + (sizeof(double) * neuron->num_coeffs);
    return data_size;
}

// Free the returned value after use
compressed_neuron_t * neuron_linear_save(neuron_params_t * n_params) {
    uint32_t data_size = sizeof(compressed_neuron_t) + (sizeof(double) * n_params->num_coeffs);
    compressed_neuron_t *n_data = calloc(data_size, 1);
    n_data->size = data_size;
    n_data->n_type = n_params->n_type;
    n_data->num_inputs = n_params->num_inputs;
    n_data->num_coeffs = n_params->num_coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        n_data->coeffs[i] = n_params->coeffs[i];
    }
    return n_data;
}

uint32_t neuron_linear_restore(neuron_params_t * n_params, compressed_neuron_t * n_data) {
    // compressed_neuron_t *n_data = restore_data(filename);
    uint32_t ret_val = n_data->size;
    // printf("Neuron: restoring %d bytes\n", ret_val);
    n_params->n_type = n_data->n_type;
    n_params->num_inputs = n_data->num_inputs;
    n_params->num_coeffs = n_data->num_coeffs;
    for(uint32_t i=0; i<n_data->num_coeffs; i++) {
        n_params->coeffs[i] = n_data->coeffs[i];
    }
    // free(n_data);
    return ret_val;
}