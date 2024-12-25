#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "neuron_smart.h"

// Linear neuron

typedef struct {
    double *inputs;
    uint32_t *indices;
    double *coeffs;     // bias is coeffs[num_inputs]
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    uint32_t feedback_error_count;
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    double last_value;
} neuron_params_t;

neuron_params_t params;

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
    if(coeff > (1.0 * params.num_inputs)) {
        return (1.0 * params.num_inputs);
    } else if(coeff < (-1.0 * params.num_inputs)) {
        return (-1.0 * params.num_inputs);
    } else {
        return coeff;
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
    for(int i=0; i<params.num_coeffs; i++) {
        params.coeffs[i] = random_double(-0.01, 0.01);
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
    params.feedback_error += error;
    params.feedback_error_count += 1;
}

DLL_PREFIX
void set_global_error(double error) {
    params.global_error = error;
}

uint32_t last_mutation_idx = 0;
double mutation_step = 0.01;
uint32_t loops = 0;
uint32_t restored = 0;
size_t bad_mutations = 0;
uint8_t mutated = 0;

DLL_PREFIX
void mutate(void) {
    double mutation_step = 0.01;
    params.last_idx = random_int(0, params.num_coeffs);
    params.last_value = params.coeffs[params.last_idx];
    double random_val = random_double(-1 * mutation_step, mutation_step);
    params.coeffs[params.last_idx] = control_coeffs_func(params.coeffs[params.last_idx] + random_val);
}

DLL_PREFIX
void restore(void) {
    params.coeffs[params.last_idx] = params.last_value;
    restored = 1;
}

DLL_PREFIX
void print_coeffs(void) {
    for(int i=0; i<params.num_coeffs; i++) {
        printf("%lf, ", params.coeffs[i]);
    }
    printf("\n");
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
    restore_data(params.inputs, params.num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(params.indices, params.num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(params.coeffs, (params.num_inputs+1) * sizeof(double), coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}
