#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"

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
    // printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
        return;
    }
    printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    exit(1);
}

void neuron_set_coeffs(neuron_params_t * n_params, double *coeffs) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = coeffs[i];
    }
}

void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, double new_value) {
    if(idx < n_params->num_coeffs) {
        n_params->coeffs[idx] = new_value;
    }
}

double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx) {
    return n_params->coeffs[idx];
}

uint32_t neuron_get_num_coeffs(neuron_params_t * n_params) {
    return n_params->num_coeffs;
}

// // Returns number of bytes written to the buffer
// int neuron_get_coeffs_as_string(neuron_params_t *n_params, char *buffer, uint32_t buffer_size) {
//     uint32_t idx = 0;
//     for(uint32_t i=0; i<n_params->num_coeffs-1; i++) {
//         idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.5f,\n", n_params->coeffs[i]);
//     }
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.5f", n_params->coeffs[n_params->num_coeffs-1]);
//     return idx;
// }

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
        printf("Error: unknown neuron type: %d; exit\n", n_params->n_type);
        exit(1);
    }
    return activation_func(output);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", n_params->coeffs[i]);
    }
    printf("\n");
}
