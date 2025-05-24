#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"
#include "linear_neuron.h"
#include "poly_neuron.h"

uint32_t neuron_get_data_size(neuron_params_t *neuron) {
    if(neuron->n_type == NLinear) {
        return neuron_linear_get_data_size(neuron);
    } else if(neuron->n_type == NPoly) {
        return neuron_poly_get_data_size(neuron);
    }
    printf("Error: Unknown neuron type: %d! %s:%d\n", neuron->n_type, __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

// Free the returned value after use
compressed_neuron_t * neuron_save(neuron_params_t * n_params) {
    if(n_params->n_type == NLinear) {
        return neuron_linear_save(n_params);
    } else if(n_params->n_type == NPoly) {
        return neuron_poly_save(n_params);
    }
    printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

uint32_t neuron_restore(neuron_params_t * n_params, compressed_neuron_t * n_data) {
    if(n_params->n_type == NLinear) {
        return neuron_linear_restore(n_params, n_data);
    } else if(n_params->n_type == NPoly) {
        return neuron_poly_restore(n_params, n_data);
    }
    printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}