#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "neuron_smart.h"

// Linear nanite

typedef struct {
    double *inputs;
    uint32_t *indices;
    double *coeffs;     // bias is coeffs[num_inputs]
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    double last_value;
} nanite_params_t;

nanite_params_t params;

double activation_func(double sum) {
    if(sum > 1.0) {
        return 1.0;
    } else if(sum < -1.0) {
        return -1.0;
    } else {
        return sum;
    }
}

DLL_PREFIX
void init(uint32_t num_inputs) {
    srand(time(NULL));
    params.num_inputs = num_inputs;
    params.num_coeffs = 1 << num_inputs;
    if(params.inputs) {
        free(params.inputs);
    }
    params.inputs = calloc(num_inputs, sizeof(double));
    if(params.indices) {
        free(params.indices);
    }
    params.indices = calloc(num_inputs, sizeof(uint32_t));
    if(params.coeffs) {
        free(params.coeffs);
    }
    params.coeffs = calloc(params.num_coeffs, sizeof(double));
    for(int i=0; i<num_inputs+1; i++) {
        params.coeffs[i] = random_double(-1.0, 1.0);
    }
}

DLL_PREFIX
void set_input_idx(uint32_t input_number, uint32_t input_idx) {
    if(input_number < params.num_inputs) {
        params.indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, params.num_inputs);
    }
}

DLL_PREFIX
double get_output(double *inputs) {
    double result = 0;
    for(size_t i=0; i<params.num_inputs; i++) {
        params.inputs[i] = inputs[i];
    }
    for(size_t i=0; i<params.num_coeffs; i++) {
        double temp = params.coeffs[i];
        for(size_t j=0; j<params.num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                temp *= inputs[j];
            }
        }
        result += temp;
    }
    return activation_func(result);
}

DLL_PREFIX
void set_feedback_error(double error) {
    params.feedback_error = error;
}

DLL_PREFIX
void set_global_error(double error) {
    params.global_error = error;
}

DLL_PREFIX
void mutate(void) {
    params.last_idx = random_int(0, params.num_coeffs);
    params.last_value = params.coeffs[params.last_idx];
    params.coeffs[params.last_idx] += random_double(-0.01, 0.01);
}

DLL_PREFIX
void restore(void) {
    params.coeffs[params.last_idx] = params.last_value;
}

DLL_PREFIX
void save_state(char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    store_data(&params, sizeof(params), filename);
    store_data(params.inputs, params.num_inputs * sizeof(double), inputs_arr_fname);
    store_data(params.indices, params.num_inputs * sizeof(uint32_t), indices_arr_fname);
    store_data(params.coeffs, (params.num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

DLL_PREFIX
void restore_state(char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    restore_data(&params, sizeof(params), filename);
    // params.inputs = malloc(params.num_inputs * sizeof(double));
    // params.indices = malloc(params.num_inputs * sizeof(uint32_t));
    // params.coeffs = malloc((params.num_inputs+1) * sizeof(double));
    restore_data(params.inputs, params.num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(params.indices, params.num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(params.coeffs, (params.num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}
