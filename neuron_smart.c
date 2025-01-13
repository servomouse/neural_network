#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"
#include "micro_net.h"

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

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs, uint32_t dataset_size) {
    // srand(time(NULL));   // Should be called by controller
    printf("Creating neuron with %d inputs\n", num_inputs);
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = 1 << num_inputs;
    n_params->num_outputs = 0;
    n_params->output_counter = 0;
    n_params->dataset_size = dataset_size;
    neuron_reset_feedback_error(n_params);
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
    if(n_params->part_values) {
        free(n_params->part_values);
    }
    n_params->part_values = calloc(n_params->num_coeffs * n_params->dataset_size, sizeof(double));
    if(n_params->outputs) {
        free(n_params->outputs);
    }
    n_params->outputs = calloc(n_params->dataset_size, sizeof(double));
}

void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx) {
    printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    }
}

void neuron_set_output_idx(neuron_params_t * n_params, uint32_t output_idx) {
    printf("Setting output index to %d\n", output_idx == NOT_OUTPUT? -1: output_idx);
    n_params->output_idx = output_idx;
}

uint32_t neuron_get_output_idx(neuron_params_t * n_params) {
    return n_params->output_idx;
}

void neuron_reset_output_counter(neuron_params_t * n_params) {
    n_params->output_counter = 0;
}

double neuron_get_output(neuron_params_t * n_params, double *inputs) {
    neuron_reset_feedback_error(n_params);
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[i];
    }
    uint32_t part_val_idx = n_params->num_coeffs * n_params->output_counter;
    n_params->part_values[part_val_idx] = 0;
    n_params->outputs[n_params->output_counter] = n_params->coeffs[0];         // BIAS
    for(size_t i=1; i<n_params->num_coeffs; i++) {  // Inputs
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                temp *= inputs[j];
            }
        }
        n_params->part_values[part_val_idx + i] = temp;
        n_params->outputs[n_params->output_counter] += temp * n_params->coeffs[i];
    }
    n_params->outputs[n_params->output_counter] = activation_func(n_params->outputs[n_params->output_counter]);
    double ret_val = n_params->outputs[n_params->output_counter];
    n_params->output_counter += 1;
    return ret_val;
}

void neuron_set_feedback_error(neuron_params_t * n_params, double error) {
    n_params->feedback_error += error;
    n_params->feedback_error_count += 1;
    n_params->num_feedbacks_received += 1;
}

void neuron_reset_feedback_error(neuron_params_t * n_params) {
    n_params->feedback_error = 0;
    n_params->feedback_error_count = 0;
    n_params->num_feedbacks_received = 0;
}

void neuron_set_global_error(neuron_params_t * n_params, double error) {
    n_params->global_error = error;
}

void neuron_set_num_outputs(neuron_params_t * n_params, uint32_t new_value) {
    n_params->num_outputs = new_value;
}

uint32_t neuron_get_num_outputs(neuron_params_t * n_params) {
    return n_params->num_outputs;
}

void neuron_update_coeffs(neuron_params_t * n_params, micro_network_t *micronet) {
    double feedback_error = 0;
    if(n_params->feedback_error_count > 0) {
        feedback_error = n_params->feedback_error / n_params->feedback_error_count;
    }
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        double new_value = 0;
        uint32_t counter = 0;
        uint32_t index = i;
        for(uint32_t step = 0; step<n_params->output_counter; step++) {
            index += n_params->num_coeffs;
            double micronet_inputs[] = {
                n_params->global_error,         // Global error
                feedback_error,                 // Feedback error
                n_params->coeffs[i],            // Current value
                n_params->part_values[index],   // Partial value calculated by the neuron_get_output function
                n_params->outputs[step]         // Neuron output
            };
            new_value += micronet_get_output(micronet, micronet_inputs);
            counter ++;
        }
        if(counter > 0) {
            // printf("Updating coeff[%d]: old value = %f, new value = %f, counter = %d, ", i, n_params->coeffs[i], new_value, counter);
            new_value = control_coeffs_func(new_value/counter);
            // printf("adjusted new value = %f\n", new_value);
            n_params->coeffs[i] = new_value;
        } else {
            printf("Error: counter == 0!!!\n");
            exit(1);
        }
        // TODO: Add feedback calculation
    }
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
    char *part_values_arr_fname = concat_strings("part_values_", filename);
    store_data(&n_params, sizeof(neuron_params_t), filename);
    store_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    store_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), coeffs_arr_fname);
    store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), part_values_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
    free(part_values_arr_fname);
}

void neuron_restore_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    char *part_values_arr_fname = concat_strings("part_values_", filename);
    restore_data(&n_params, sizeof(neuron_params_t), filename);
    restore_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), coeffs_arr_fname);
    restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), part_values_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
    free(part_values_arr_fname);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", n_params->coeffs[i]);
    }
    printf("\n");
}
