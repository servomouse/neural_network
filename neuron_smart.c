#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"
#include "micro_net.h"
#include "neurons_structures.h"

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

#define BUF_SIZE 2048

void neuron_prepare_file(const char *filename) {
    char buffer[BUF_SIZE] = {0};
    uint32_t idx = 0;

    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "#include <stdint.h>\n\n");

    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "typedef struct {\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tuint32_t num_inputs;\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tuint32_t num_coeffs;\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tuint32_t dataset_size;\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tdouble *coeffs;\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tuint32_t *indices;\n");
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "}neuron_min_desc_t;\n\n");

    FILE *file = fopen(filename, "wb");
    if ((file != NULL) && (idx < BUF_SIZE)) {
        fprintf (file, "%s", buffer);
        fclose(file);
    } else {
        printf("File write error!");
    }
}

void neuron_save_coeffs(neuron_params_t * n_params, uint32_t neuron_idx) {
    char buffer[BUF_SIZE] = {0};
    snprintf(buffer, BUF_SIZE, "data/neuron_%d_coeffs.bin", neuron_idx);
    store_data(n_params->coeffs, n_params->num_coeffs * sizeof(double), buffer);
}

void neuron_save_data(neuron_params_t * n_params, const char *filename, uint32_t neuron_idx) {
    neuron_save_coeffs(n_params, neuron_idx);
    char buffer[BUF_SIZE] = {0};
    uint32_t idx = 0;

    // Write coeffs:
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double coeffs_%d[%d] = {\n", neuron_idx, n_params->num_coeffs);
    for(uint32_t i=0; i<n_params->num_coeffs-1; i++) {
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%.15f,\n", n_params->coeffs[i]);
    }
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%.15f\n};\n\n", n_params->coeffs[n_params->num_coeffs-1]);

    // Write indices:
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "uint32_t indices_%d[%d] = {", neuron_idx, n_params->num_inputs);
    for(uint32_t i=0; i<n_params->num_inputs-1; i++) {
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "%d, ", n_params->indices[i]);
    }
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "%d};\n\n", n_params->indices[n_params->num_inputs-1]);

    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "neuron_min_desc_t neuron_%d = {\n", neuron_idx);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t.num_inputs = %d,\n", n_params->num_inputs);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t.num_coeffs = %d,\n", n_params->num_coeffs);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t.dataset_size = %d,\n", n_params->dataset_size);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t.coeffs = coeffs_%d,\n", neuron_idx);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t.indices = indices_%d,\n", neuron_idx);

    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "};\n\n");

    FILE *file = fopen(filename, "ab");
    if ((file != NULL) && (idx < BUF_SIZE)) {
        fprintf (file, "%s", buffer);
        fclose(file);
    } else {
        printf("File write error, here are coeffs: %s", buffer);
    }
}

void neuron_complete_file(const char *filename, uint32_t num_neurons) {
    char buffer[BUF_SIZE] = {0};
    uint32_t idx = 0;

    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "neuron_min_desc_t *saved_neurons[] = {\n");
    for(uint32_t i=0; i<num_neurons-1; i++) {
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t&neuron_%d,\n", i);
    }
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t&neuron_%d\n};\n", num_neurons-1);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "uint8_t restore_from_header =  0;\t // Set to 1 to restore these values\n");

    FILE *file = fopen(filename, "ab");
    if ((file != NULL) && (idx < BUF_SIZE)) {
        fprintf (file, "%s", buffer);
        fclose(file);
    } else {
        printf("File write error!");
    }
}

void neuron_restore_data(neuron_params_t * n_params, uint32_t neuron_idx) {
    if(restore_from_header) {
        // printf("Restoring neuron %d from header\n", neuron_idx);
        neuron_min_desc_t *neuron = saved_neurons[neuron_idx];
        // n_params->num_inputs = neuron->num_inputs;
        // n_params->num_coeffs = neuron->num_coeffs;
        // n_params->dataset_size = neuron->dataset_size;
        for(int i=0; i<n_params->num_coeffs; i++) {
            n_params->coeffs[i] = neuron->coeffs[i];
        }
        // for(size_t i=0; i<n_params->num_inputs; i++) {
        //    n_params->indices[i] = neuron->indices[i];
        // }
    } else {
        // printf("Restoring neuron %d from binary\n", neuron_idx);
        char buffer[BUF_SIZE] = {0};
        snprintf(buffer, BUF_SIZE, "data/neuron_%d_coeffs.bin", neuron_idx);
        restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double), buffer);
    }
}

void * alloc_memory(void *p, size_t num_elements, size_t sizeof_element) {
    if(p) {
        free(p);
    }
    return calloc(num_elements, sizeof_element);
}

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs, uint32_t dataset_size) {
    // srand(time(NULL));   // Should be called by controller
    printf("Creating neuron with %d inputs\n", num_inputs);
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = 1 << num_inputs;
    n_params->num_outputs = 0;
    n_params->output_counter = 0;
    n_params->dataset_size = dataset_size;
    n_params->mutation_step = 0.01;
    neuron_reset_feedback_error(n_params);

    n_params->inputs = alloc_memory(n_params->inputs, num_inputs, sizeof(double));
    n_params->indices = alloc_memory(n_params->indices, num_inputs, sizeof(uint32_t));
    n_params->coeffs = alloc_memory(n_params->coeffs, n_params->num_coeffs, sizeof(double));
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = random_double(-0.1, 0.1);
    }

    if(n_params->backup_coeffs) {
        free(n_params->backup_coeffs);
    }
    n_params->backup_coeffs = calloc(n_params->num_coeffs, sizeof(double));
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->backup_coeffs[i] = n_params->coeffs[i];
    }

    if(n_params->last_vector) {
        free(n_params->last_vector);
    }
    n_params->last_vector = calloc(n_params->num_coeffs, sizeof(double));

    if(n_params->coeff_feedback) {
        free(n_params->coeff_feedback);
    }
    n_params->coeff_feedback = calloc(n_params->num_coeffs, sizeof(double));
    n_params->inputs_feedback = alloc_memory(n_params->inputs_feedback, n_params->num_inputs, sizeof(feedback_item_t*));
    for(uint32_t i=0; i<n_params->dataset_size; i++) {
        n_params->inputs_feedback[i] = calloc(n_params->dataset_size, sizeof(feedback_item_t));
    }

    if(n_params->rand_vector) {
        free(n_params->rand_vector);
    }
    n_params->rand_vector = calloc(n_params->num_coeffs, sizeof(double));

    if(n_params->part_values) {
        free(n_params->part_values);
    }
    n_params->part_values = calloc(n_params->num_coeffs * n_params->dataset_size, sizeof(double));
    if(n_params->outputs) {
        free(n_params->outputs);
    }
    n_params->outputs = calloc(n_params->dataset_size, sizeof(double));
    if(n_params->global_errors) {
        free(n_params->global_errors);
    }
    n_params->global_errors = calloc(n_params->dataset_size, sizeof(double));
}

void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx) {
    printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    }
}

void neuron_set_coeffs(neuron_params_t * n_params, double *coeffs) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = coeffs[i];
    }
}

// Returns number of bytes written to the buffer
int neuron_get_coeffs_as_string(neuron_params_t *n_params, char *buffer, uint32_t buffer_size) {
    for(uint32_t i=0; i<buffer_size; i++) {
        buffer[i] = 0;
    }
    uint32_t idx = 0;
    for(uint32_t i=0; i<n_params->num_coeffs-1; i++) {
        idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.15f,\n", n_params->coeffs[i]);
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t%.15f", n_params->coeffs[n_params->num_coeffs-1]);
    return idx;
}

void neuron_backup(neuron_params_t *n_params) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->backup_coeffs[i] = n_params->coeffs[i];
    }
}

void neuron_check_backup(neuron_params_t *n_params) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        if(n_params->coeffs[i] != n_params->backup_coeffs[i]) {
            printf("Backup check error: coeff[%d] = %f, backup_coeff[%d] = %f!\n", i, n_params->coeffs[i], i, n_params->backup_coeffs[i]);
        }
    }
}

void neuron_restore(neuron_params_t *n_params) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = n_params->backup_coeffs[i];
    }
}

void neuron_set_output_idx(neuron_params_t * n_params, uint32_t output_idx) {
    printf("Setting output index to %d\n", output_idx == NOT_OUTPUT? -1: output_idx);
    n_params->output_idx = output_idx;
}

uint32_t neuron_get_output_idx(neuron_params_t * n_params) {
    return n_params->output_idx;
}

void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, double new_value) {
    if(idx < n_params->num_coeffs) {
        n_params->coeffs[idx] = new_value;
    }
}

void neuron_reset_output_counter(neuron_params_t * n_params) {
    n_params->output_counter = 0;
}

double neuron_get_output(neuron_params_t * n_params, double *inputs, uint32_t to_print) {
    // to_print = 0;
    double part_values[32] = {0};
    neuron_reset_feedback_error(n_params);
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[n_params->indices[i]];
    }
    uint32_t part_val_idx = n_params->num_coeffs * n_params->output_counter;
    n_params->part_values[part_val_idx] = 0;
    n_params->outputs[n_params->output_counter] = n_params->coeffs[0];         // BIAS
    // if(to_print)
    //     printf("BIAS: %f\n", n_params->coeffs[0]);
    for(size_t i=1; i<n_params->num_coeffs; i++) {  // Inputs
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                temp *= n_params->inputs[j];
            }
        }
        part_values[i] = temp;
        // if(to_print)
        //     printf("Part value %lld: %f; coeff: %f\n", i, temp, n_params->coeffs[i]);
        n_params->part_values[part_val_idx + i] = temp;
        n_params->outputs[n_params->output_counter] += temp * n_params->coeffs[i];
    }
    n_params->outputs[n_params->output_counter] = activation_func(n_params->outputs[n_params->output_counter]);
    double ret_val = n_params->outputs[n_params->output_counter];
    if(to_print) {
        printf("\"part_values\": [");
        for(size_t i=0; i<n_params->num_coeffs-1; i++) {
            printf("%f, ", part_values[i]);
        }
        printf("%f];\n", part_values[n_params->num_coeffs-1]);
        printf("\"coeffs\": [");
        for(size_t i=0; i<n_params->num_coeffs-1; i++) {
            printf("%f, ", n_params->coeffs[i]);
        }
        printf("%f];\n", n_params->coeffs[n_params->num_coeffs-1]);
        printf("\"output\": %f\n\n", ret_val);
    }
    // n_params->output_counter += 1;
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

void neuron_set_global_error(neuron_params_t * n_params, double error, uint32_t offset) {
    // printf("Global error: %f\n", error);
    n_params->global_errors[offset] = error;
    n_params->output_counter += 1;
}

void neuron_set_num_outputs(neuron_params_t * n_params, uint32_t new_value) {
    n_params->num_outputs = new_value;
}

uint32_t neuron_get_num_outputs(neuron_params_t * n_params) {
    return n_params->num_outputs;
}

uint32_t neuron_get_num_coeffs(neuron_params_t * n_params) {
    return n_params->num_coeffs;
}

void neuron_update_coeffs(neuron_params_t * n_params, micro_network_t *coeffs_micronet, micro_network_t *feedback_micronet, feedback_item_t **local_errors, uint32_t own_index) {
    // double feedback_error = 0;
    // if(feedback_arr[own_index].counter > 0) {
    //     feedback_error = feedback_arr[own_index].value / feedback_arr[own_index].counter;
    // }
    
    for(size_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeff_feedback[i] = 0;
    }
    for(size_t i=0; i<n_params->num_inputs; i++) {
        for(uint32_t j=0; j<n_params->dataset_size; j++) {
            n_params->inputs_feedback[i][j].value = 0;
            n_params->inputs_feedback[i][j].counter = 0;
        }
    }
    // printf("Updating coeffs of the neuron[%d]\n", own_index);
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        double coeff_value = 0, feedback_value = 0;
        uint32_t index = i;
        for(uint32_t step = 0; step<n_params->output_counter; step++) {
            index += n_params->num_coeffs;
            double micronet_inputs[5] = {
                n_params->global_errors[step],  // Global error
                local_errors[own_index][step].value/local_errors[own_index][step].counter,  // Feedback error
                n_params->coeffs[i],            // Current value
                n_params->part_values[index],   // Partial value calculated by the neuron_get_output function
                n_params->outputs[step]         // Neuron output
            };
            coeff_value    += micronet_get_output(coeffs_micronet, micronet_inputs);
            feedback_value += micronet_get_output(feedback_micronet, micronet_inputs);
        }
        if(n_params->output_counter > 0) {
            n_params->coeffs[i] += coeff_value/n_params->output_counter;
            n_params->coeff_feedback[i] += feedback_value/n_params->output_counter;
        } else {
            printf("Error: n_params->output_counter == 0!!!\n");
            exit(1);
        }
    }
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double inputs_sum = 0.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                if(n_params->inputs[j] < 0)
                    inputs_sum -= n_params->inputs[j];
                else
                    inputs_sum += n_params->inputs[j];
            }
        }
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                n_params->input_feedback[j].value += n_params->inputs[j] / inputs_sum;
                n_params->input_feedback[j].counter ++;
            }
        }
    }
    for(size_t j=0; j<n_params->num_inputs; j++) {
        if(n_params->input_feedback[j].counter > 0) {
            local_errors[n_params->indices[j]].value += n_params->input_feedback[j].value / n_params->input_feedback[j].counter;
            local_errors[n_params->indices[j]].counter ++;
        }
    }
}

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

void neuron_rollback(neuron_params_t * n_params) {
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = n_params->last_vector[i];
    }
    if(n_params->mutated == 1) {
        n_params->bad_mutations_counter ++;
        n_params-> mutated = 0;
    }
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
