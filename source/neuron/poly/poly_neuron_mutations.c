#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "poly_neuron.h"

static double control_coeffs_func(double coeff, double min_value, double max_value) {
    if(coeff > max_value) {
        return max_value;
    } else if(coeff < min_value) {
        return min_value;
    } else {
        return coeff;
    }
}

// The opposite is neuron_rollback
void neuron_poly_stash_state(neuron_params_t * n_params) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    complex_coeff_t *last_vector = (complex_coeff_t*)n_params->last_vector;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        last_vector[i].val[MUL] = coeffs[i].val[MUL];
        last_vector[i].val[DIV] = coeffs[i].val[DIV];
    }
}

void gen_random_1d_vector(neuron_params_t * n_params) {
    complex_coeff_t *rand_vector = (complex_coeff_t*)n_params->rand_vector;
    uint32_t idx = random_int(0, n_params->num_coeffs);
    uint32_t mul_div = random_int(0, 2);
    double val = random_double(0, n_params->mutation_step);
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        rand_vector[i].val[0] = 0;
        rand_vector[i].val[1] = 0;
    }
    rand_vector[idx].val[mul_div] = val;
}

void neuron_poly_mutate(neuron_params_t * n_params) {
    neuron_poly_stash_state(n_params);
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    complex_coeff_t *rand_vector = (complex_coeff_t*)n_params->rand_vector;
    
    if(n_params->mutated == 1) {    // If previuos mutation was successfull, keep going in the same direction
        n_params->bad_mutations_counter = 0;
    } else {
        if(n_params->bad_mutations_counter >= 10000) {
            // If there were many unsuccessfull mutations, try a large mutation
            gen_complex_vector(n_params->num_coeffs, random_double(0, 1), rand_vector);
            n_params->bad_mutations_counter = 0;
        } else {
            gen_complex_vector(n_params->num_coeffs, random_double(0, n_params->mutation_step), rand_vector);
            // gen_random_1d_vector(n_params);
        }
    }
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i].val[MUL] = control_coeffs_func(coeffs[i].val[MUL] + rand_vector[i].val[MUL], -1.0, 1.0);
        coeffs[i].val[DIV] = control_coeffs_func(coeffs[i].val[MUL] + rand_vector[i].val[DIV], DIV_LOW_LIMIT, 1.0);
    }
    n_params->mutated = 1;
}

// The opposite is neuron_stash_state
void neuron_poly_rollback(neuron_params_t * n_params) {
    complex_coeff_t *coeffs = (complex_coeff_t*)n_params->coeffs;
    complex_coeff_t *last_vector = (complex_coeff_t*)n_params->last_vector;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i].val[MUL] = last_vector[i].val[MUL];
        coeffs[i].val[DIV] = last_vector[i].val[DIV];
    }
    if(n_params->mutated == 1) {
        n_params->bad_mutations_counter ++;
        n_params-> mutated = 0;
    }
}
