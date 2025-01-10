#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"

double act_table[] = {
    1.0000, 0.9951, 0.9720, 0.9358, 0.8895, 0.8363, 0.7794, 0.7214, 0.6644, 0.6098,
    0.5586, 0.5111, 0.4677, 0.4282, 0.3924, 0.3601, 0.3311, 0.3049, 0.2814, 0.2602,
    0.2411, 0.2238, 0.2082, 0.1941, 0.1813, 0.1696, 0.1590, 0.1493, 0.1405, 0.1324,
    0.1249, 0.1181, 0.1117, 0.1059, 0.1005, 0.0955, 0.0909, 0.0866, 0.0826, 0.0789,
    0.0754, 0.0721, 0.0691, 0.0662, 0.0635, 0.0610, 0.0586, 0.0564, 0.0543, 0.0523,
    0.0504, 0.0487, 0.0470, 0.0454, 0.0439, 0.0424, 0.0411, 0.0398, 0.0385, 0.0374,
    0.0362, 0.0352, 0.0341, 0.0332, 0.0322, 0.0313, 0.0305, 0.0296, 0.0288, 0.0281,
    0.0274, 0.0267, 0.0260, 0.0253, 0.0247, 0.0241, 0.0235, 0.0230, 0.0224, 0.0219,
    0.0214, 0.0209, 0.0204, 0.0200, 0.0195, 0.0191, 0.0187, 0.0182, 0.0178, 0.0175,
    0.0171, 0.0167, 0.0164, 0.0160, 0.0157, 0.0154, 0.0150, 0.0147, 0.0144, 0.0141,
    0.0138, 0.0135, 0.0133, 0.0130, 0.0127, 0.0125, 0.0122, 0.0120, 0.0117, 0.0115,
    0.0112, 0.0110, 0.0108, 0.0105, 0.0103, 0.0101, 0.0099, 0.0097, 0.0095, 0.0093,
    0.0091, 0.0089, 0.0087, 0.0085, 0.0083, 0.0081, 0.0079, 0.0077, 0.0075, 0.0072,
    0.0071, 0.0070, 0.0069, 0.0068, 0.0067, 0.0066, 0.0065, 0.0064, 0.0063, 0.0062,
    0.0061, 0.0060, 0.0059, 0.0058, 0.0057, 0.0056, 0.0055, 0.0054, 0.0053, 0.0052,
    0.0051, 0.0050, 0.0049, 0.0048, 0.0047, 0.0046, 0.0045, 0.0044, 0.0043, 0.0042,
    0.0041, 0.0040, 0.0039, 0.0038, 0.0037, 0.0036, 0.0035, 0.0034, 0.0033, 0.0032,
    0.0031, 0.0030, 0.0029, 0.0028, 0.0027, 0.0026, 0.0025, 0.0024, 0.0023, 0.0022,
    0.0021, 0.0020, 0.0019, 0.0018, 0.0017, 0.0016, 0.0015, 0.0014, 0.0013, 0.0012,
    0.0011, 0.0010, 0.0009, 0.0008, 0.0007, 0.0006, 0.0005, 0.0004, 0.0003, 0.0002,
    0.0001, 0.0000
};

double get_act_value(double offset, double amplitude, double sum) {
    sum += offset;
    if(sum < 0) {
        sum = 0 - sum;
    }
    double temp_sum = sum * 100;
    double xa = floor(temp_sum);
    double xb = ceil(temp_sum);
    uint8_t bot = (uint8_t)xa;
    uint8_t top = (uint8_t)xb;
    if(top == bot) {
        return act_table[bot];
    }
    double ya = act_table[bot];
    double yb = act_table[top];
    double res = (((yb - ya) * (temp_sum - xa)) / (xb - xa)) + ya;
    return res * amplitude;
}


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
    n_params->num_coeffs = (1 << num_inputs);
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
    n_params->coeffs = calloc(n_params->num_coeffs, 2 * sizeof(double));
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i].amplitude = random_double(-0.01, 0.01);
        n_params->coeffs[i].offset = random_double(-0.01, 0.01);
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
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[i];
    }

    double result = get_act_value(n_params->coeffs[0].offset, n_params->coeffs[0].amplitude, 0);    // BIAS
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i) > 0) {
                temp *= inputs[j];
            }
        }
        double output = get_act_value(n_params->coeffs[i].offset, n_params->coeffs[i].amplitude, temp);
        if(isnan(output)) {
            printf("temp is NAN!!!: offset = %f, amplitude = %f, temp = %f, output = %f\n", n_params->coeffs[i].offset, n_params->coeffs[i].amplitude, temp, output);
        }
        if(isnan(result + output)) {
            printf("Result + output is NAN:: result = %f, output = %f", result, output);
        }
        result += output;
        if(isnan(result)) {
            printf("Result is NAN:: result = %f, output = %f", result, output);
        }
    }
    result = activation_func(result);
    if(isnan(activation_func(result))) {
        printf("Result for %f is NAN!!:\n", result);
        for(size_t i=0; i<n_params->num_inputs; i++) {
            printf("inputs[%lld] = %f\n", i, n_params->inputs[i]);
        }
    }
    return result;
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
    n_params->last_value.amplitude = n_params->coeffs[n_params->last_idx].amplitude;
    n_params->last_value.offset = n_params->coeffs[n_params->last_idx].offset;
    n_params->coeffs[n_params->last_idx].amplitude = control_coeffs_func(
        n_params->coeffs[n_params->last_idx].amplitude + random_double(-1 * mutation_step, mutation_step));
    n_params->coeffs[n_params->last_idx].offset = control_coeffs_func(
        n_params->coeffs[n_params->last_idx].offset + random_double(-1 * mutation_step, mutation_step));
}

void neuron_restore(neuron_params_t * n_params) {
    n_params->coeffs[n_params->last_idx].amplitude = n_params->last_value.amplitude;
    n_params->coeffs[n_params->last_idx].offset = n_params->last_value.offset;
}

void neuron_save_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    store_data(&n_params, sizeof(neuron_params_t), filename);
    store_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    store_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double) * 2, coeffs_arr_fname);
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
    restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double) * 2, coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, %f; ", n_params->coeffs[i].amplitude, n_params->coeffs[i].offset);
    }
    printf("\n");
}
