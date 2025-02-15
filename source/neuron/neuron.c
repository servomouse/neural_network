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

static double control_coeffs_func(double coeff) {
    if(coeff > 1.0) {
        return 1.0;
    } else if(coeff < -1.0) {
        return -1.0;
    } else {
        return coeff;
    }
}

static void * alloc_memory(void *p, size_t num_elements, size_t sizeof_element) {
    if(p) { free(p); }
    return calloc(num_elements, sizeof_element);
}

void neuron_init(neuron_params_t * n_params, neuron_type_t n_type, uint32_t num_inputs) {
    // srand(time(NULL));   // Should be called by controller
    printf("Creating neuron with %d inputs\n", num_inputs);
    n_params->n_type = n_type;
    n_params->num_inputs = num_inputs;
    if(n_type == NLinear) {
        n_params->num_coeffs = num_inputs+1;    // +1 for BIAS
    } else if((n_type == NPoly) || (n_type == NSmart)) {
        n_params->num_coeffs = 1 << num_inputs;
    } else {
        printf("Error: unknown neuron type: %d; exit\n", n_type);
        exit(1);
    }
    n_params->mutation_step = 0.01;
	// n_params->num_outputs = 0;
    // n_params->output_counter = 0;
    // n_params->inputs_feedback_counter = 0;
    // neuron_reset_feedback_error(n_params);

    n_params->inputs = alloc_memory(n_params->inputs, n_params->num_inputs, sizeof(double));
    // n_params->inputs = alloc_memory(n_params->direct_inputs, n_params->num_inputs, sizeof(double));
    // n_params->inputs = alloc_memory(n_params->micronet_msg, 5, sizeof(double));
    // n_params->input_feedbacks = alloc_memory(n_params->input_feedbacks, n_params->num_inputs, sizeof(double));
    n_params->indices = alloc_memory(n_params->indices, n_params->num_inputs, sizeof(uint32_t));

    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(double));
    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(double));
    n_params->coeffs = alloc_memory(n_params->coeffs, n_params->num_coeffs, sizeof(double));
    n_params->backup_coeffs = alloc_memory(n_params->backup_coeffs, n_params->num_coeffs, sizeof(double));
    n_params->part_feedbacks = alloc_memory(n_params->part_feedbacks, n_params->num_coeffs, sizeof(double));
    n_params->part_sums = alloc_memory(n_params->part_sums, n_params->num_coeffs, sizeof(double));
    n_params->feedback_micronet_stash = alloc_memory(n_params->feedback_micronet_stash, n_params->num_coeffs * MICRONET_STASH_SIZE, sizeof(double));
    n_params->coeffs_micronet_stash   = alloc_memory(n_params->coeffs_micronet_stash, n_params->num_coeffs * MICRONET_STASH_SIZE, sizeof(double));
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = random_double(-0.1, 0.1);
        n_params->backup_coeffs[i] = n_params->coeffs[i];
    }

    n_params->last_vector = alloc_memory(n_params->last_vector, n_params->num_coeffs, sizeof(double));

    // if(n_params->coeff_feedback) {
    //     free(n_params->coeff_feedback);
    // }
    // n_params->coeff_feedback = calloc(n_params->num_coeffs, sizeof(double));
    // n_params->inputs_feedback = alloc_memory(n_params->inputs_feedback, n_params->num_inputs, sizeof(complex_value_t));
    // for(uint32_t i=0; i<n_params->dataset_size; i++) {
    //     n_params->inputs_feedback[i] = calloc(n_params->dataset_size, sizeof(complex_item_t));
    // }

    n_params->rand_vector = alloc_memory(n_params->rand_vector, n_params->num_coeffs, sizeof(double));

    // if(n_params->part_values) {
    //     free(n_params->part_values);
    // }
    // n_params->part_values = calloc(n_params->num_coeffs, sizeof(double));
    // if(n_params->outputs) {
    //     free(n_params->outputs);
    // }
    // n_params->outputs = calloc(n_params->dataset_size, sizeof(complex_item_t));
    // if(n_params->global_errors) {
    //     free(n_params->global_errors);
    // }
    // n_params->global_errors = calloc(n_params->dataset_size, sizeof(double));
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

// Returns number of bytes written to the buffer
int neuron_get_coeffs_as_string(neuron_params_t *n_params, char *buffer, uint32_t buffer_size) {
    uint32_t idx = 0;
    for(uint32_t i=0; i<n_params->num_coeffs-1; i++) {
        idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.5f,\n", n_params->coeffs[i]);
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.5f", n_params->coeffs[n_params->num_coeffs-1]);
    return idx;
}

// The opposite is neuron_restore
void neuron_backup(neuron_params_t *n_params) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->backup_coeffs[i] = n_params->coeffs[i];
    }
}

// The opposite is neuron_backup
void neuron_restore(neuron_params_t *n_params) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = n_params->backup_coeffs[i];
    }
}

void neuron_clear_stashes(neuron_params_t * n_params) {
    for(size_t i=0; i<n_params->num_coeffs * MICRONET_STASH_SIZE; i++) {
        n_params->feedback_micronet_stash[i] = 0.0;
    }
    for(size_t i=0; i<n_params->num_coeffs * MICRONET_STASH_SIZE; i++) {
        n_params->coeffs_micronet_stash[i] = 0.0;
    }
}

double neuron_get_output(neuron_params_t *n_params, double *inputs) {
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[n_params->indices[i]];
    }
    double output;
    if(n_params->n_type == NLinear) {
        output = n_params->coeffs[n_params->num_inputs];         // BIAS
        for(size_t i=0; i<n_params->num_inputs; i++) {
            uint32_t idx = n_params->indices[i];
            output += inputs[idx] * n_params->coeffs[i];
        }
    } else if((n_params->n_type == NPoly) || (n_params->n_type == NSmart)) {
        output = n_params->coeffs[0];         // BIAS
        for(size_t i=1; i<n_params->num_coeffs; i++) {
            double temp = 1.0;
            for(size_t j=0; j<n_params->num_inputs; j++) {
                if(((1 << j) & i)> 0) {
                    uint32_t idx = n_params->indices[j];
                    temp *= inputs[idx];
                }
            }
            output += temp * n_params->coeffs[i];
        }
    } else {
        printf("Error: unknown neuron type: %d; exit\n", n_params->n_type);
        exit(1);
    }
    return activation_func(output);
}

// double neuron_get_output(neuron_params_t * n_params, double *inputs) {
//     // double part_values[256] = {0};
//     for(size_t i=0; i<n_params->num_inputs; i++) {
//         n_params->inputs[i] = inputs[n_params->indices[i]];
//         // n_params->part_sums[i] = 0.0;
//     }

//     double output = n_params->coeffs[0];         // BIAS
//     for(size_t i=1; i<n_params->num_coeffs; i++) {
//         double temp = 1.0;
//         for(size_t j=0; j<n_params->num_inputs; j++) {
//             if(((1 << j) & i) > 0) {
//                 temp *= n_params->inputs[j];
//                 // n_params->part_sums[j] += n_params->inputs[j];
//             }
//         }
//         temp *= n_params->coeffs[i];
//         n_params->part_values[i] = activation_func(temp);
//         output += temp * n_params->coeffs[i];
//     }
//     n_params->output = activation_func(output);
//     if(n_params->output != n_params->output) {
//         printf("Error! Coeffs:");
//         for(uint32_t i=0; i<n_params->num_coeffs; i++) {
//             printf("%f, ", n_params->coeffs[i]);
//         }
//         printf("\n");
//         exit(0);
//     }
//     if(to_print) {
//         printf("\"part_values\": [");
//         for(size_t i=0; i<n_params->num_coeffs-1; i++) {
//             printf("%f, ", n_params->part_values[i]);
//         }
//         printf("%f];\n", n_params->part_values[n_params->num_coeffs-1]);
//         printf("\"coeffs\": [");
//         for(size_t i=0; i<n_params->num_coeffs-1; i++) {
//             printf("%f, ", n_params->coeffs[i]);
//         }
//         printf("%f];\n", n_params->coeffs[n_params->num_coeffs-1]);
//         printf("\"output\": %f\n\n", n_params->output);
//     }
//     return n_params->output;
// }

// void neuron_set_feedback_error(neuron_params_t * n_params, double error) {
//     n_params->feedback_error += error;
//     n_params->feedback_error_count += 1;
//     n_params->num_feedbacks_received += 1;
// }

// void neuron_reset_feedback_error(neuron_params_t * n_params) {
//     n_params->feedback_error = 0;
//     n_params->feedback_error_count = 0;
//     n_params->num_feedbacks_received = 0;
// }

void neuron_set_global_error(neuron_params_t * n_params, double error) {
    // printf("Global error: %f\n", error);
    n_params->global_error = error;
}

uint32_t neuron_get_num_coeffs(neuron_params_t * n_params) {
    return n_params->num_coeffs;
}

// The opposite is neuron_rollback
void neuron_stash_state(neuron_params_t * n_params) {
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->last_vector[i] = n_params->coeffs[i];
    }
}

void neuron_mutate(neuron_params_t * n_params) {
    neuron_stash_state(n_params);
    
    if(n_params->mutated == 1) {    // If previuos mutation was successfull, keep going in the same direction
        n_params->bad_mutations_counter = 0;
    } else {
        if(n_params->bad_mutations_counter >= 10000) {
            gen_vector(n_params->num_coeffs, random_double(0, 1), n_params->rand_vector);
        } else {
            gen_vector(n_params->num_coeffs, random_double(0, n_params->mutation_step), n_params->rand_vector);
        }
    }
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = control_coeffs_func(n_params->coeffs[i] + n_params->rand_vector[i]);
    }
    n_params->mutated = 1;
}

// The opposite is neuron_rollback
void neuron_rollback(neuron_params_t * n_params) {
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = n_params->last_vector[i];
    }
    if(n_params->mutated == 1) {
        n_params->bad_mutations_counter ++;
        n_params-> mutated = 0;
    }
}

// void neuron_save_state(neuron_params_t * n_params, char *filename) {
//     char *inputs_arr_fname = concat_strings("inputs_", filename);
//     char *indices_arr_fname = concat_strings("indices_", filename);
//     char *coeffs_arr_fname = concat_strings("coeffs_", filename);
//     char *part_values_arr_fname = concat_strings("part_values_", filename);
//     store_data(&n_params, sizeof(neuron_params_t), filename);
//     store_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
//     store_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
//     store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), coeffs_arr_fname);
//     store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), part_values_arr_fname);
//     free(inputs_arr_fname);
//     free(indices_arr_fname);
//     free(coeffs_arr_fname);
//     free(part_values_arr_fname);
// }

// void neuron_restore_state(neuron_params_t * n_params, char *filename) {
//     char *inputs_arr_fname = concat_strings("inputs_", filename);
//     char *indices_arr_fname = concat_strings("indices_", filename);
//     char *coeffs_arr_fname = concat_strings("coeffs_", filename);
//     char *part_values_arr_fname = concat_strings("part_values_", filename);
//     restore_data(&n_params, sizeof(neuron_params_t), filename);
//     restore_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
//     restore_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
//     restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), coeffs_arr_fname);
//     restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), part_values_arr_fname);
//     free(inputs_arr_fname);
//     free(indices_arr_fname);
//     free(coeffs_arr_fname);
//     free(part_values_arr_fname);
// }

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", n_params->coeffs[i]);
    }
    printf("\n");
}
