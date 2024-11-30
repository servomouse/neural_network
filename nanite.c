#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "nanite.h"

// Linear nanite

typedef struct {
    double *inputs;
    uint32_t *indices;
    double *coeffs;     // bias is coeffs[num_inputs]
    uint32_t num_inputs;
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
    params.inputs = calloc(num_inputs, sizeof(double));
    params.indices = calloc(num_inputs, sizeof(uint32_t));
    params.coeffs = calloc(num_inputs+1, sizeof(double));
    for(int i=0; i<num_inputs+1; i++) {
        params.coeffs[i] = random_double(-1.0, 1.0);
    }
}

DLL_PREFIX
void set_input_idx(uint32_t input_number, uint32_t input_idx) {
    params.indices[input_number] = input_idx;
}

DLL_PREFIX
double get_output(double *inputs) {
    double result = params.coeffs[params.num_inputs];
    for(int i=0; i<params.num_inputs; i++) {
        params.inputs[i] = inputs[i];
        result += inputs[i] * params.coeffs[i];
    }
    return activation_func(result);
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
    params.inputs = malloc(params.num_inputs * sizeof(double));
    params.indices = malloc(params.num_inputs * sizeof(uint32_t));
    params.coeffs = malloc((params.num_inputs+1) * sizeof(double));
    restore_data(params.inputs, params.num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(params.indices, params.num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(params.coeffs, (params.num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}