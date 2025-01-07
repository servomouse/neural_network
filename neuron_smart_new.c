#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron_smart_new.h"

double act_table[] = {
    1.0000, 0.9976, 0.9905, 0.9788, 0.9628, 0.9429, 0.9194, 0.8929, 0.8638, 0.8327, 
    0.8000, 0.7662, 0.7317, 0.6968, 0.6620, 0.6275, 0.5936, 0.5604, 0.5281, 0.4968, 
    0.4665, 0.4375, 0.4096, 0.3829, 0.3574, 0.3330, 0.3098, 0.2878, 0.2668, 0.2469, 
    0.2279, 0.2100, 0.1929, 0.1767, 0.1613, 0.1467, 0.1329, 0.1197, 0.1072, 0.0953, 
    0.0839, 0.0732, 0.0629, 0.0532, 0.0439, 0.0350, 0.0265, 0.0184, 0.0107, 0.0033, 
    -0.0038, -0.0105, -0.0170, -0.0232, -0.0292, -0.0349, -0.0405, -0.0457, -0.0508, -0.0558, 
    -0.0605, -0.0651, -0.0695, -0.0737, -0.0778, -0.0818, -0.0857, -0.0894, -0.0931, -0.0966, 
    -0.1000, -0.1033, -0.1066, -0.1097, -0.1128, -0.1158, -0.1187, -0.1215, -0.1243, -0.1271, 
    -0.1297, -0.1323, -0.1349, -0.1374, -0.1399, -0.1423, -0.1446, -0.1470, -0.1492, -0.1515, 
    -0.1537, -0.1559, -0.158, -0.16010, -0.1622, -0.1643, -0.1663, -0.1683, -0.1702, -0.1722, -0.1741, -0.1760
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


// double activation_func(double sum) {
//     return get_act_value(sum);
// }

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
    double result = get_act_value(n_params->coeffs[0].offset, n_params->coeffs[0].amplitude, 0);
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[i];
    }
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                temp *= inputs[j];
            }
        }
        result += get_act_value(n_params->coeffs[i].offset, n_params->coeffs[i].amplitude, temp);
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
