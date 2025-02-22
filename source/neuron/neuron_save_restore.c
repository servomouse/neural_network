#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"

typedef struct {
    neuron_type_t n_type;
    uint32_t num_inputs;
    uint32_t num_coeffs;
    double coeffs[0];
} compressed_neuron_t;

void neuron_save(neuron_params_t * n_params, char *filename) {
    uint32_t data_size = sizeof(compressed_neuron_t) + (sizeof(double) * n_params->num_coeffs);
    compressed_neuron_t *n_data = calloc(data_size, 1);
    n_data->n_type = n_params->n_type;
    n_data->num_inputs = n_params->num_inputs;
    n_data->num_coeffs = n_params->num_coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        n_data->coeffs[i] = n_params->coeffs[i];
    }
    store_data(n_data, data_size, filename);
    free(n_data);
}

void neuron_restore(neuron_params_t * n_params, char *filename) {
    compressed_neuron_t *n_data = restore_data(filename);
    n_params->n_type = n_data->n_type;
    n_params->num_inputs = n_data->num_inputs;
    n_params->num_coeffs = n_data->num_coeffs;
    for(uint32_t i=0; i<n_data->num_coeffs; i++) {
        n_params->coeffs[i] = n_data->coeffs[i];
    }
    free(n_data);
}