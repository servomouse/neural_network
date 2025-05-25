#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"
#include "poly/poly_neuron.h"
#include "linear/linear_neuron.h"

static double activation_func(double sum) {
    if(sum > 1.0) {
        return 1.0;
    } else if(sum < -1.0) {
        return -1.0;
    } else {
        return sum;
    }
}

void neuron_init(neuron_params_t * n_params, neuron_type_t n_type, uint32_t num_inputs) {
    // srand(time(NULL));   // Should be called by controller
    if(n_type == NLinear) {
        neuron_linear_init(n_params, num_inputs);
    } else if(n_type == NPoly) {
        neuron_poly_init(n_params, num_inputs);
    } else {
        printf("Error: unknown neuron type: %d; exit\n", n_type);
        exit(1);
    }
}

void neuron_set_input_idx(neuron_params_t *n_params, uint32_t input_number, uint32_t input_idx) {
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
        return;
    }
    printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    exit(1);
}

void neuron_set_coeffs(neuron_params_t * n_params, double *new_coeffs) {
    double *coeffs = (double*)n_params->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        coeffs[i] = new_coeffs[i];
    }
}

void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, double new_value) {
    double *coeffs = (double*)n_params->coeffs;
    if(idx < n_params->num_coeffs) {
        coeffs[idx] = new_value;
    }
}

double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx) {
    double *coeffs = (double*)n_params->coeffs;
    return coeffs[idx];
}

double neuron_get_output(neuron_params_t *n_params, double *inputs) {
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[n_params->indices[i]];
    }
    double output;
    if(n_params->n_type == NLinear) {
        output = neuron_linear_get_output(n_params, inputs);
    } else if(n_params->n_type == NPoly) {
        output = neuron_poly_get_output(n_params, inputs);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    return activation_func(output);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    double *coeffs = (double*)n_params->coeffs;
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", coeffs[i]);
    }
    printf("\n");
}
