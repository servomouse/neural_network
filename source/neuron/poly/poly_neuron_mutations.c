#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "poly_neuron.h"

static double control_coeffs_func(double coeff) {
    if(coeff > 1.0) {
        return 1.0;
    } else if(coeff < -1.0) {
        return -1.0;
    } else {
        return coeff;
    }
}

// The opposite is neuron_rollback
void neuron_poly_stash_state(neuron_params_t * n_params) {
    double *coeffs = (double*)n_params->coeffs;
    double *last_vector = (double*)n_params->last_vector;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        last_vector[i] = coeffs[i];
    }
}

void neuron_poly_mutate(neuron_params_t * n_params) {
    neuron_poly_stash_state(n_params);
    double *coeffs = (double*)n_params->coeffs;
    double *rand_vector = (double*)n_params->rand_vector;
    
    if(n_params->mutated == 1) {    // If previuos mutation was successfull, keep going in the same direction
        n_params->bad_mutations_counter = 0;
    } else {
        if(n_params->bad_mutations_counter >= 10000) {
            // If there was many unsuccessfull mutations, try a large mutation
            gen_vector(n_params->num_coeffs, random_double(0, 1), rand_vector);
        } else {
            gen_vector(n_params->num_coeffs, random_double(0, n_params->mutation_step), rand_vector);
        }
    }
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i] = control_coeffs_func(coeffs[i] + rand_vector[i]);
    }
    n_params->mutated = 1;
}

// The opposite is neuron_stash_state
void neuron_poly_rollback(neuron_params_t * n_params) {
    double *coeffs = (double*)n_params->coeffs;
    double *last_vector = (double*)n_params->last_vector;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i] = last_vector[i];
    }
    if(n_params->mutated == 1) {
        n_params->bad_mutations_counter ++;
        n_params-> mutated = 0;
    }
}
