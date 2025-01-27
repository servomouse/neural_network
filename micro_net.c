#include <stdlib.h>
#include "micro_net.h"
#include "neuron.h"
#include "utils.h"
#include <time.h>

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

// May be only single output
void micronet_init(micro_network_t * config, micronet_map_t *net_map, double **bckp_coeffs) {
    config->map = net_map;
    config->num_inputs = net_map->num_inputs;
    config->num_neurons = net_map->num_neurons;
    config->net_size = net_map->net_size;
    config->output_idx = net_map->output_idx;
    config->mutated_neuron_idx = 0;
    printf("Creating micro network with:\n");
    printf("\t%d inputs;\n", config->num_inputs);
    printf("\t%d neurons;\n", config->num_neurons);
    printf("\ttotal size is %d\n", config->net_size);

    config->arr = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(neuron_params_t));
    for(size_t i=0; i<config->num_neurons; i++) {
        uint8_t idx = net_map->neurons[i].idx - config->num_inputs;
        uint8_t num_inputs = net_map->neurons[i].num_inputs;
        neuron_init(&config->neurons[idx], num_inputs, 1);
        for(size_t j=0; j<net_map->neurons[i].num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, net_map->neurons[i].indices[j]);
            if(j >= config->num_inputs) {
                uint32_t temp_idx = j-config->num_inputs;
                neuron_set_num_outputs(&config->neurons[temp_idx], neuron_get_num_outputs(&config->neurons[temp_idx]) + 1);
            }
        }
        if(bckp_coeffs) {
            neuron_set_coeffs(&config->neurons[idx], bckp_coeffs[idx]);
        }
    }
}

double micronet_get_output(micro_network_t * config, double *inputs) {
    for(int i=0; i<config->num_neurons; i++) {
        neuron_reset_output_counter(&config->neurons[i]);
    }
    // printf("Micronet array: ");
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr, 0);
        }
        // printf("%f, ", config->arr[i]);
    }
    // printf("\n");
    return config->arr[config->output_idx];
}

void micronet_set_global_error(micro_network_t *config, double error) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_set_global_error(&config->neurons[i-config->num_inputs], error);
    }
}

void micronet_save(micro_network_t * config, char *filename) {
    char *arr_fname = concat_strings("arr_", filename);
    store_data(&config, sizeof(config), filename);
    store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void micronet_restore(micro_network_t * config, char *filename) {
    if(!config->neurons) {
        printf("Call init() before calling restore_state()!");
        exit(1);
    }
    char *arr_fname = concat_strings("arr_", filename);
    restore_data(&config, sizeof(config), filename);
    restore_data(config->arr, config->net_size * sizeof(double), arr_fname);
    free(arr_fname);
}

void micronet_mutate(micro_network_t * config) {
    // config->mutated_neuron_idx = random_int(0, config->num_neurons);
    config->mutated_neuron_idx++;
    if(config->mutated_neuron_idx == config->num_neurons) {
        config->mutated_neuron_idx = 0;
    }
    neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_rollback(micro_network_t * config) {
    neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_print_coeffs(micro_network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}

// Buffer is supposed to be empty, returns number of bytes written to the buffer
int micronet_save_map(micro_network_t * config, char *buffer, uint32_t buffer_size, char *prefix) {
    uint32_t idx = 0;

    // idx += snprintf(&buffer[idx], buffer_size-idx, "#pragma once\n");
    // idx += snprintf(&buffer[idx], buffer_size-idx, "#include \"neuron_types.h\"\n\n");
    idx += snprintf(&buffer[idx], buffer_size-idx, "micronet_map_t %sbackup_map = {\n", prefix);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_inputs = %d,\n", config->num_inputs);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_neurons = %d,\n", config->num_neurons);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.net_size = %d,\n", config->net_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.output_idx = %d,\n", config->output_idx);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.neurons = {\n");

    for(uint32_t i=0; i<config->num_neurons; i++) {
        subneuron_description_t *n = &config->map->neurons[i];
        idx += snprintf(&buffer[idx], buffer_size-idx, "\t\t{.idx = %d, ", n->idx);
        idx += snprintf(&buffer[idx], buffer_size-idx, ".num_inputs = %d, ", n->num_inputs);
        idx += snprintf(&buffer[idx], buffer_size-idx, ".indices = {");
        for(uint32_t j=0; j<n->num_inputs-1; j++) {
            idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", n->indices[j]);
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d}}", n->indices[n->num_inputs-1]);
        if(i < config->num_neurons-1) {
            idx += snprintf(&buffer[idx], buffer_size-idx, ",");
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "\n");
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t}\n};\n\n");
    return idx;
}

#define BUF_SIZE 2048

void micronet_save_data(micro_network_t * config, char *filename, char *prefix) {
    // char *arr_fname = concat_strings("arr_", filename);
    // store_data(&config, sizeof(config), filename);
    // store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
    // free(arr_fname);
    // char filename_buf[] = "neurons_structures.h";
    char buffer[BUF_SIZE] = {0};
    micronet_save_map(config, buffer, BUF_SIZE, prefix);
    
    write_buf_to_file(buffer, filename);

    for(uint32_t i=0; i<config->num_neurons; i++) {
        uint32_t idx = snprintf(buffer, BUF_SIZE, "double %scoeffs_%04d[%d] = {\n", prefix, i, neuron_get_num_coeffs(&config->neurons[i]));
        idx += neuron_get_coeffs_as_string(&config->neurons[i], &buffer[idx], BUF_SIZE-idx);
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\n};\n\n");
        write_buf_to_file(buffer, filename);
    }
    clear_buffer(buffer, BUF_SIZE);

    uint32_t idx = 0;
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double *%sbackup_coeffs[] = {\n", prefix);
    for(uint32_t i=0; i<config->num_neurons-1; i++) {
        idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d,\n", prefix, i);
    }
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d\n", prefix, config->num_neurons-1);
    idx += snprintf(&buffer[idx], BUF_SIZE-idx, "};\n\n");
    write_buf_to_file(buffer, filename);

    // neuron_prepare_file(filename_buf);
    // for(uint32_t i=0; i<config->num_neurons; i++) {
    //     // snprintf(&filename_buf[0], 20, "data/neurons.h", i);
    //     neuron_save_data(&config->neurons[i], filename_buf, i);
    // }
    // neuron_complete_file(filename_buf, config->num_neurons);
}
