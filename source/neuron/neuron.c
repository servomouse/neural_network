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
        exit(EXIT_FAILURE);
    }
}

void neuron_set_input_idx(neuron_params_t *n_params, uint32_t input_number, uint32_t input_idx) {
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
        n_params->inputs_set = 1;
        return;
    }
    printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    exit(EXIT_FAILURE);
}

double neuron_get_output(neuron_params_t *n_params, double *inputs) {
    if(n_params->inputs_set == 0) {
        printf("Error: input indices was not set!\n");
        exit(EXIT_FAILURE);
    }
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[n_params->indices[i]];
    }
    double output;
    if(n_params->n_type == NLinear) {
        output = neuron_linear_get_output(n_params);
    } else if(n_params->n_type == NPoly) {
        output = neuron_poly_get_output(n_params);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    return activation_func(output);
}

void neuron_set_coeffs(neuron_params_t * n_params, void *new_coeffs) {
    if(n_params->n_type == NLinear) {
        neuron_linear_set_coeffs(n_params, new_coeffs);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_set_coeffs(n_params, new_coeffs);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, void *new_coeff) {
    if(n_params->n_type == NLinear) {
        neuron_linear_set_coeff(n_params, idx, new_coeff);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_set_coeff(n_params, idx, new_coeff);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx) {
    if(n_params->n_type == NLinear) {
        return neuron_linear_get_coeff(n_params, idx);
    } else if(n_params->n_type == NPoly) {
        return neuron_poly_get_coeff(n_params, idx);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    if(n_params->n_type == NLinear) {
        neuron_linear_print_coeffs(n_params);
    } else if(n_params->n_type == NPoly) {
        neuron_poly_print_coeffs(n_params);
    } else {
        printf("Error: Unknown neuron type: %d! %s:%d\n", n_params->n_type, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
