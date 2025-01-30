#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron_simple.h"
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


void * alloc_memory(void *p, size_t num_elements, size_t sizeof_element) {
    if(p) {
        free(p);
    }
    return calloc(num_elements, sizeof_element);
}

void neuron_init(simple_neuron_params_t * n_params, uint32_t num_inputs, uint32_t dataset_size) {
    // srand(time(NULL));   // Should be called by controller
    printf("Creating simple neuron with %d inputs\n", num_inputs);
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = 1 << num_inputs;
    n_params->num_outputs = 0;
    n_params->output_counter = 0;
    n_params->inputs_feedback_counter = 0;
    n_params->dataset_size = dataset_size;
    n_params->mutation_step = 0.01;
    // neuron_reset_feedback_error(n_params);

    n_params->inputs = alloc_memory(n_params->inputs, n_params->num_inputs, sizeof(complex_item_t));
    n_params->inputs = alloc_memory(n_params->direct_inputs, n_params->num_inputs, sizeof(double));
    n_params->inputs = alloc_memory(n_params->micronet_msg, 5, sizeof(double));
    n_params->input_feedbacks = alloc_memory(n_params->input_feedbacks, n_params->num_inputs, sizeof(double));
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
    n_params->inputs_feedback = alloc_memory(n_params->inputs_feedback, n_params->num_inputs, sizeof(double));
    // for(uint32_t i=0; i<n_params->dataset_size; i++) {
    //     n_params->inputs_feedback[i] = calloc(n_params->dataset_size, sizeof(complex_item_t));
    // }

    if(n_params->rand_vector) {
        free(n_params->rand_vector);
    }
    n_params->rand_vector = calloc(n_params->num_coeffs, sizeof(double));

    if(n_params->part_values) {
        free(n_params->part_values);
    }
    n_params->part_values = calloc(n_params->num_coeffs * n_params->dataset_size, sizeof(complex_item_t));
    // if(n_params->outputs) {
    //     free(n_params->outputs);
    // }
    // n_params->outputs = calloc(n_params->dataset_size, sizeof(complex_item_t));
    // if(n_params->global_errors) {
    //     free(n_params->global_errors);
    // }
    // n_params->global_errors = calloc(n_params->dataset_size, sizeof(double));
}

void neuron_set_input_idx(simple_neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx) {
    printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    }
}

void neuron_set_coeffs(simple_neuron_params_t * n_params, double *coeffs) {
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = coeffs[i];
    }
}

// Returns number of bytes written to the buffer
int neuron_get_coeffs_as_string(simple_neuron_params_t *n_params, char *buffer, uint32_t buffer_size) {
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

double neuron_get_output(simple_neuron_params_t *n_params, double *inputs) {

    double output = n_params->coeffs[0];         // BIAS
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                uint32_t idx = n_params->indices[i];
                temp *= inputs[idx];
            }
        }
        output += temp * n_params->coeffs[i];
    }
    return activation_func(output);
}

uint32_t neuron_get_num_coeffs(simple_neuron_params_t * n_params) {
    return n_params->num_coeffs;
}

void neuron_stash_state(simple_neuron_params_t * n_params) {
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->last_vector[i] = n_params->coeffs[i];
    }
}

void neuron_mutate(simple_neuron_params_t * n_params) {
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

void neuron_rollback(simple_neuron_params_t * n_params) {
    for(int32_t i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i] = n_params->last_vector[i];
    }
    if(n_params->mutated == 1) {
        n_params->bad_mutations_counter ++;
        n_params-> mutated = 0;
    }
}

void neuron_print_coeffs(simple_neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, ", n_params->coeffs[i]);
    }
    printf("\n");
}
