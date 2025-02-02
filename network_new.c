#include <stdlib.h>
#include <string.h>
#include "micro_net.h"
#include "network_new.h"
#include "neuron.h" // includes neuron_types.h
#include "utils.h"
#include <time.h>
#include "network_backup.h"

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

uint32_t neurons[] = {
    // idx  num_inputs  indices
       4,   5,          0, 1, 2, 3, 4,
       5,   5,          0, 1, 2, 3, 4,
       6,   5,          0, 1, 2, 3, 4,
       7,   5,          0, 1, 2, 3, 4,
       8,   5,          0, 1, 2, 3, 4,
       9,   5,          0, 1, 2, 3, 4,
      10,   5,          5, 6, 7, 8, 9
};

micronet_map_t coeffs_micronet_map = {
    .num_inputs = 5,
    .num_neurons = 6,
    .net_size = 11,
    .neurons = neurons,
    .num_outputs = 1,
    .output_indices = {10},
};

neuron_params_t *neurons_backup;
double *arr_backup;

void network_init(network_t * config, network_map_t *net_map, uint32_t dataset_size) {
    srand(time(NULL));
    config->map = net_map;
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->net_size - net_map->num_inputs;
    config->net_size = net_map->net_size;
    config->num_outputs = net_map->num_outputs;
    config->dataset_size = dataset_size;
    printf("Creating network with:\n");
    printf("\t%d inputs;\n", config->num_inputs);
    printf("\t%d output(s);\n", config->num_outputs);
    printf("\t%d neurons;\n", config->num_neurons);
    printf("\ttotal size is %d\n", config->net_size);
    
    config->arr = calloc(config->net_size, sizeof(double));
    arr_backup = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    config->feedback_errors = calloc(config->net_size, sizeof(complex_item_t));
    config->feedback_ = calloc(config->net_size, sizeof(complex_value_t));
    config->feedback_activations = calloc(config->net_size, sizeof(complex_item_t));
    neurons_backup = calloc(config->num_neurons, sizeof(neuron_params_t));
    config->output_indices = calloc(config->num_outputs, sizeof(uint32_t));
    config->outputs = calloc(config->num_outputs, sizeof(double));
    
    uint32_t offset = 0;
    for(size_t i=0; i<config->num_neurons; i++) {
        neuron_desc_t *neuron = (neuron_desc_t *)&net_map->neurons[offset];
        uint8_t idx = neuron->idx - config->num_inputs;
        uint8_t num_inputs = neuron->num_inputs;
        neuron_init(&config->neurons[idx], num_inputs, dataset_size);
        neuron_set_output_idx(&config->neurons[idx], neuron->output_idx);
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, neuron->indices[j]);
            if(j >= config->num_inputs) {
                uint32_t temp_idx = j-config->num_inputs;
                neuron_set_num_outputs(&config->neurons[temp_idx], neuron_get_num_outputs(&config->neurons[temp_idx]) + 1);
            }
        }
        #ifdef USE_BACKUP
        neuron_set_coeffs(&config->neurons[idx], network_backup_coeffs[idx]);
        #endif
        if(neuron->output_idx != NOT_OUTPUT) {
            config->output_indices[neuron->output_idx] = config->num_inputs + i;
        }
        offset += num_inputs + 3;
    }
    // Init micronets:
    config->feedback_micronet = calloc(1, sizeof(micro_network_t));
    config->coeffs_micronet = calloc(1, sizeof(micro_network_t));
    // #undef USE_BACKUP
    #ifdef USE_BACKUP
    micronet_init(config->feedback_micronet, &feedback_micronet_backup_map, feedback_micronet_backup_coeffs);
    micronet_init(config->coeffs_micronet, &coeffs_micronet_backup_map, coeffs_micronet_backup_coeffs);
    #else
    micronet_init(config->feedback_micronet, &feedback_micronet_map, NULL);
    micronet_init(config->coeffs_micronet, &coeffs_micronet_map, NULL);
    #endif
}

void network_backup(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_backup(&config->neurons[i]);
    }
    // memcpy(neurons_backup, config->neurons, sizeof(neuron_params_t) * config->num_neurons);
    // memcpy(arr_backup, config->arr, sizeof(double) * config->net_size);
}

void network_check_backup(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_check_backup(&config->neurons[i]);
    }
}

void network_restore(network_t * config) {
    // memcpy(config->neurons, neurons_backup, sizeof(neuron_params_t) * config->num_neurons);
    // memcpy(config->arr, arr_backup, sizeof(double) * config->net_size);
    for(int i=0; i<config->num_neurons; i++) {
        neuron_restore(&config->neurons[i]);
    }
    printf("Network restored!\n");
}

double* network_get_outputs(network_t * config, double *inputs, uint32_t to_print) {
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            if(to_print) {
                printf("{\n");
            }
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr, config->feedback_activations, config->num_inputs+i, to_print);
            if(to_print) {
                printf("},\n");
            }
        }
    }
    for(size_t i=0; i<config->num_outputs; i++) {
        config->outputs[i] = config->arr[config->output_indices[i]];
    }
    return config->outputs;
}

void network_reset_activations(network_t *config) {
    for(int i=0; i<config->net_size; i++) {
        config->arr[i] = 0;
        config->feedback[i].value = 0;
        config->feedback[i].counter = 0;
    }
}

void network_set_global_error(network_t * config, double error) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_set_global_error(&config->neurons[i], error);
    }
}

void network_set_local_errors(network_t * config, double *errors) {
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->feedback[config->output_indices[i]].value = errors[i];
        config->feedback[config->output_indices[i]].counter++;
    }
    for(int i=config->num_neurons-1; i>=0; i--) {
        neuron_generate_feedbacks(&config->neurons[i], config->feedback);
    }
}

void network_update_weights(network_t *config) {
    for(int i=config->num_neurons-1; i>=0; i--) {
        neuron_update_weights(&config->neurons[i], config->feedback);
    }
}

// Buffer is supposed to be empty, returns number of bytes written to the buffer
int network_save_map(network_t * config, char *buffer, uint32_t buffer_size) {
    uint32_t idx = 0;

    idx += snprintf(&buffer[idx], buffer_size-idx, "#pragma once\n");
    idx += snprintf(&buffer[idx], buffer_size-idx, "#include \"neuron_types.h\"\n\n");
    idx += snprintf(&buffer[idx], buffer_size-idx, "network_map_t backup_map = {\n");
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_inputs = %d,\n", config->num_inputs);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.net_size = %d,\n", config->net_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_outputs = %d,\n", config->num_outputs);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.neurons = {\n");

    uint32_t offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        idx += snprintf(&buffer[idx], buffer_size-idx, "\t\t%d,\t", config->map->neurons[offset++]);   // Index
        uint32_t num_inputs = config->map->neurons[offset];
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d,\t", config->map->neurons[offset++]);   // Num_inputs
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d,\t", config->map->neurons[offset++]);   // Output index
        for(uint32_t j=0; j<num_inputs-1; j++) {
            idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", config->map->neurons[offset++]);   // Input indices
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d", config->map->neurons[offset++]);   // Last input index of a neuron
        if(i < config->num_neurons-1) {
            idx += snprintf(&buffer[idx], buffer_size-idx, ",");
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "\n");
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t}\n};\n");
    return idx;
}

#define BUF_SIZE 2048

void network_save_data(network_t * config, char *filename) {
    // char *arr_fname = concat_strings("arr_", filename);
    // store_data(&config, sizeof(config), filename);
    // store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
    // free(arr_fname);
    // char filename_buf[] = "neurons_structures.h";
    char buffer[BUF_SIZE] = {0};
    network_save_map(config, buffer, BUF_SIZE);
    
    write_buf_to_file(buffer, filename);

    for(uint32_t i=0; i<config->num_neurons; i++) {
        uint32_t idx = snprintf(buffer, BUF_SIZE, "double network_coeffs_%04d[%d] = {\n", i, neuron_get_num_coeffs(&config->neurons[i]));
        idx += neuron_get_coeffs_as_string(&config->neurons[i], &buffer[idx], BUF_SIZE-idx);
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\n};\n\n");
        write_buf_to_file(buffer, filename);
    }
    clear_buffer(buffer, BUF_SIZE);

    uint32_t idx = 0;
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double *network_backup_coeffs[] = {\n");
    for(uint32_t i=0; i<config->num_neurons-1; i++) {
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tnetwork_coeffs_%04d,\n", i);
    }
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\tnetwork_coeffs_%04d\n", config->num_neurons-1);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "};\n\n");
    write_buf_to_file(buffer, filename);
    micronet_save_data(config->feedback_micronet, filename, "feedback_micronet_");
    micronet_save_data(config->coeffs_micronet, filename, "coeffs_micronet_");

    write_buf_to_file("#define USE_BACKUP\n", filename);

    // neuron_prepare_file(filename_buf);
    // for(uint32_t i=0; i<config->num_neurons; i++) {
    //     // snprintf(&filename_buf[0], 20, "data/neurons.h", i);
    //     neuron_save_data(&config->neurons[i], filename_buf, i);
    // }
    // neuron_complete_file(filename_buf, config->num_neurons);
}

void network_restore_data(network_t * config, char *filename) {
    // if(!config->neurons) {
    //     printf("Call init() before calling restore_state()!");
    //     exit(1);
    // }
    // char *arr_fname = concat_strings("arr_", filename);
    // restore_data(&config, sizeof(config), filename);
    // restore_data(config->arr, config->net_size * sizeof(double), arr_fname);
    // free(arr_fname);
    for(uint32_t i=0; i<config->num_neurons; i++) {
        neuron_restore_data(&config->neurons[i], i);
    }
}

void network_stash_neurons(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_stash_state(&config->neurons[i]);
    }
}

void network_update_neurons(network_t * config) {
    // for(int i=0; i<config->num_neurons; i++) {
    //     config->feedback_array[i].counter = 0;
    //     config->feedback_array[i].value = 0.0;
    // }
    for(int i=config->num_neurons-1; i>=0; i--) {   // Go backwards
        neuron_update_coeffs(&config->neurons[i],
                             config->coeffs_micronet,
                             config->feedback_micronet,
                             config->feedback_errors,
                             config->feedback_activations, config->num_inputs+i);
    }
}

void network_rollback_neurons(network_t * config) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_rollback(&config->neurons[i]);
    }
}

void network_mutate(network_t * config) {
    config->mutated_neuron_idx = random_int(0, config->num_neurons);
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void network_rollback(network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}

void network_mutate_micronet(network_t * config) {
    config->last_mutated_micronet = random_bit();
    if(config->last_mutated_micronet) {
        micronet_mutate(config->feedback_micronet);
    } else {
        micronet_mutate(config->coeffs_micronet);
    }
}

void network_rollback_micronet(network_t * config) {
    if(config->last_mutated_micronet) {
        micronet_rollback(config->feedback_micronet);
    } else {
        micronet_rollback(config->coeffs_micronet);
    }
}

void network_print_coeffs(network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
