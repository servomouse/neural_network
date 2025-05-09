#include <stdlib.h>
#include <string.h>
#include "micronet.h"
#include "network.h"
#include "neuron.h" // includes neuron_types.h
#include "utils.h"
#include <time.h>
#include "neuron_iface.h"
// #include "backups/network_backup.h"

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

neuron_params_t *neurons_backup;
double *arr_backup;

void network_init(network_t *net, net_config_t *net_config) {
    srand(time(NULL));
    net->map = net_config->net_map;
    net->num_inputs = net_map->num_inputs;
    net->num_neurons = net_map->net_size - net_map->num_inputs;
    net->net_size = net_map->net_size;
    net->num_outputs = net_map->num_outputs;
    // net->dataset_size = dataset_size;
    printf("Creating network with:\n");
    printf("\t%d inputs;\n", net->num_inputs);
    printf("\t%d output(s);\n", net->num_outputs);
    printf("\t%d neurons;\n", net->num_neurons);
    printf("\ttotal size is %d\n", net->net_size);
    
    net->arr = calloc(net->net_size, sizeof(double));
    net->feedback = calloc(net->net_size, sizeof(complex_value_t));
    net->arr_backup = calloc(net->net_size, sizeof(double));
    net->neurons = calloc(net->num_neurons, sizeof(neuron_params_t));
    net->feedback_errors = calloc(net->net_size, sizeof(complex_item_t));
    net->feedback_activations = calloc(net->net_size, sizeof(complex_item_t));
    neurons_backup = calloc(net->num_neurons, sizeof(neuron_params_t));
    net->output_indices = calloc(net->num_outputs, sizeof(uint32_t));
    net->outputs = calloc(net->num_outputs, sizeof(double));

    uint32_t offset = 0;
    for(size_t i=0; i<net->num_neurons; i++) {
        neuron_desc_t *neuron = (neuron_desc_t *)&net_map->neurons[offset];
        uint8_t idx = neuron->idx - net->num_inputs;
        uint8_t num_inputs = neuron->num_inputs;
        neuron_init(&net->neurons[idx], 0, num_inputs);  // FIXME: Set neuron type properly
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&net->neurons[idx], j, neuron->indices[j]);
        }
        if(neuron->output_idx != NOT_OUTPUT) {
            net->output_indices[neuron->output_idx] = net->num_inputs + i;
        }
        offset += num_inputs + 3;
    }
    net->coeffs_linear_micronet = net_config->linear_micronet_map;
    net->coeffs_poly_micronet = net_config->poly_micronet_map;
    net->feedback_micronet = net_config->feedback_micronet_map;
}

void network_backup(network_t * config) {
    memcpy(arr_backup, config->arr, sizeof(double) * config->net_size);
}

void network_restore(network_t * config) {
    memcpy(config->arr, arr_backup, sizeof(double) * config->net_size);
    printf("Network restored!\n");
}

double *network_get_outputs(network_t *config, double *inputs, uint32_t to_print) {
    for(uint32_t i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            if(to_print) {
                printf("{\n");
            }
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr);
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
    for(uint32_t i=0; i<config->net_size; i++) {
        config->arr[i] = 0;
        config->feedback[i].value = 0;
        config->feedback[i].counter = 0;
    }
    for(uint32_t i=0; i<config->num_neurons; i++) {
        neuron_clear_stashes(&config->neurons[i]);
    }
}

void network_set_global_error(network_t * config, double error) {
    for(uint32_t i=0; i<config->num_neurons; i++) {
        neuron_set_global_error(&config->neurons[i], error);
    }
}

void network_set_output_errors(network_t * config, double *errors) {
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->feedback[config->output_indices[i]].value = errors[i];
        config->feedback[config->output_indices[i]].counter++;
    }
}

void network_generate_feedbacks(network_t * config) {
    for(int i=config->num_neurons-1; i>=0; i--) {
        neuron_generate_feedbacks(&config->neurons[i], config->feedback, config->feedback_micronet, i+config->num_inputs);
    }
}

void network_update_weights(network_t *config) {
    for(int i=config->num_neurons-1; i>=0; i--) {
        neuron_update_coeffs(&config->neurons[i], config->feedback, config->coeffs_micronet, i+config->num_inputs);
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
    // micronet_save_data(config->feedback_micronet, filename, "feedback_micronet_");
    // micronet_save_data(config->coeffs_micronet, filename, "coeffs_micronet_");

    write_buf_to_file("#define USE_BACKUP\n", filename);
}

void network_stash_neurons(network_t * config) {
    for(uint32_t i=0; i<config->num_neurons; i++) {
        neuron_stash_state(&config->neurons[i]);
    }
}

void network_rollback_neurons(network_t * config) {
    for(uint32_t i=0; i<config->num_neurons; i++) {
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
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}
