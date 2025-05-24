#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "linear_neuron.h"
#include "poly_neuron.h"

// The opposite is neuron_rollback
void neuron_stash_state(neuron_params_t * n_params) {
    if(n_params->n_type == NLinear) {
        neuron_linear_stash_state(n_params);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_stash_state(n_params);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void neuron_mutate(neuron_params_t * n_params) {
    if(n_params->n_type == NLinear) {
        neuron_linear_mutate(n_params);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_mutate(n_params);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

// The opposite is neuron_stash_state
void neuron_rollback(neuron_params_t * n_params) {
    if(n_params->n_type == NLinear) {
        neuron_linear_rollback(n_params);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_rollback(n_params);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
