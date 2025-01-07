#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"


double activation_func(double sum) {
    if(sum > 1.0) {
        return 1.0;
    } else if(sum < -1.0) {
        return -1.0;
    } else {
        return sum;
    }
}

double control_coeffs_func(double coeff) {
    if(coeff > 1.0) {
        return 1.0;
    } else if(coeff < -1.0) {
        return -1.0;
    } else {
        return coeff;
    }
}

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs) {
    // srand(time(NULL));   // Should be called by kapellmeister
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = 1 << num_inputs;
    if(n_params->inputs) {
        free(n_params->inputs);
    }
    n_params->inputs = calloc(num_inputs, sizeof(double));
    if(n_params->indices) {
        free(n_params->indices);
    }
    n_params->indices = calloc(num_inputs, sizeof(uint32_t));
    if(n_params->coeffs) {
        free(n_params->coeffs);
    }
    n_params->coeffs = calloc(n_params->num_coeffs, sizeof(double));
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = random_double(-0.01, 0.01);
    }
}

void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx) {
    printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    }
}

double neuron_get_output(neuron_params_t * n_params, double *inputs) {
    double result = 0;
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[i];
    }
    for(size_t i=0; i<n_params->num_coeffs; i++) {
        double temp = n_params->coeffs[i];
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                temp *= inputs[j];
            }
        }
        result += temp;
    }
    return activation_func(result);
}

void neuron_set_feedback_error(neuron_params_t * n_params, double error) {
    n_params->feedback_error += error;
    n_params->feedback_error_count += 1;
}

void neuron_set_global_error(neuron_params_t * n_params, double error) {
    n_params->global_error = error;
}

void neuron_mutate(neuron_params_t * n_params) {
    double mutation_step = 0.01;
    n_params->last_idx = random_int(0, n_params->num_coeffs);
    n_params->last_value = n_params->coeffs[n_params->last_idx];
    double random_val = random_double(-1 * mutation_step, mutation_step);
    n_params->coeffs[n_params->last_idx] = control_coeffs_func(n_params->coeffs[n_params->last_idx] + random_val);
}

void neuron_restore(neuron_params_t * n_params) {
    n_params->coeffs[n_params->last_idx] = n_params->last_value;
}

void neuron_save_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    store_data(&n_params, sizeof(neuron_params_t), filename);
    store_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    store_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    store_data(n_params->coeffs, (n_params->num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

void neuron_restore_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    restore_data(&n_params, sizeof(neuron_params_t), filename);
    restore_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(n_params->coeffs, (n_params->num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", n_params->coeffs[i]);
    }
    printf("\n");
}
