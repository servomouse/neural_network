#include <stdlib.h>
#include "micro_net.h"
#include "neuron_simple.h"
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
    config->num_outputs = net_map->num_outputs;
    config->mutated_neuron_idx = 0;
    // printf("Creating micro network with:\n");
    // printf("\t%d inputs;\n", config->num_inputs);
    // printf("\t%d neurons;\n", config->num_neurons);
    // printf("\tnum outputs: %d;\n", config->num_outputs);

    config->output_indices = calloc(config->num_outputs, sizeof(uint32_t));
    config->outputs = calloc(config->num_outputs, sizeof(double));
    // printf("\toutput indices: [");
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->output_indices[i] = net_map->output_indices[i];
        // if(i == config->num_outputs-1)
        //     printf("%d]\n", net_map->output_indices[i]);
        // else
        //     printf("%d, ", net_map->output_indices[i]);
    }
    // printf("\ttotal micronet size is %d\n", config->net_size);

    config->arr     = calloc(config->net_size, sizeof(double));
    config->neurons = calloc(config->num_neurons, sizeof(simple_neuron_params_t));

    uint32_t offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        subneuron_description_t *neuron = (subneuron_description_t *)&net_map->neurons[offset];
        uint32_t idx        = neuron->idx - config->num_inputs;
        uint32_t num_inputs = neuron->num_inputs;
        // printf("Neuron %d: idx = %d, num_inputs = %d, offset = %d\n", i, idx, num_inputs, offset);
        mini_neuron_init(&config->neurons[idx], num_inputs);
        for(size_t j=0; j<num_inputs; j++) {
            mini_neuron_set_input_idx(&config->neurons[idx], j, neuron->indices[j]);
        }
        // if(bckp_coeffs) {
        //     mini_neuron_set_coeffs(&config->neurons[idx], bckp_coeffs[idx]);
        // }
        offset += 2 + num_inputs;
    }
}

double *micronet_get_output(micro_network_t * config, double *inputs) {
    // for(int i=0; i<config->num_neurons; i++) {
    //     mini_neuron_reset_output_counter(&config->neurons[i]);
    // }
    // printf("Micronet array: ");
    for(int i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = mini_neuron_get_output(&config->neurons[i-config->num_inputs], config->arr);
        }
        // printf("%f, ", config->arr[i]);
    }
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->outputs[i] = config->arr[config->output_indices[i]];
    }
    // printf("\n");
    return config->outputs;
}

void micronet_set_global_error(micro_network_t *config, double error) {
    // for(int i=config->num_inputs; i<config->net_size; i++) {
    //     mini_neuron_set_global_error(&config->neurons[i-config->num_inputs], error);
    // }
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
    mini_neuron_mutate(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_rollback(micro_network_t * config) {
    mini_neuron_rollback(&config->neurons[config->mutated_neuron_idx]);
}

void micronet_print_coeffs(micro_network_t * config) {
    for(int i=config->num_inputs; i<config->net_size; i++) {
        mini_neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}

#define CHECK_BUFFER(_idx, _buf_size) {if(_idx >= (_buf_size-1)) {printf("ERROR: Buffer is too small! Exit\n"); exit(-1);}}

// Buffer is supposed to be empty, returns number of bytes written to the buffer
int micronet_save_map(micro_network_t * config, char *buffer, uint32_t buffer_size, char *prefix) {
    uint32_t idx = 0;

    // idx += snprintf(&buffer[idx], buffer_size-idx, "#pragma once\n");
    // idx += snprintf(&buffer[idx], buffer_size-idx, "#include \"neuron_types.h\"\n\n");
    idx += snprintf(&buffer[idx], buffer_size-idx, "micronet_map_t %sbackup_map = {\n", prefix);
    CHECK_BUFFER(idx, buffer_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_inputs = %d,\n", config->num_inputs);
    CHECK_BUFFER(idx, buffer_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_neurons = %d,\n", config->num_neurons);
    CHECK_BUFFER(idx, buffer_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.net_size = %d,\n", config->net_size);
    CHECK_BUFFER(idx, buffer_size);
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_outputs = %d,\n", config->num_outputs);
    CHECK_BUFFER(idx, buffer_size);

    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.output_indices = {");
    CHECK_BUFFER(idx, buffer_size);
    for(uint32_t i=0; i<config->num_outputs-1; i++) {
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", config->output_indices[i]);
    CHECK_BUFFER(idx, buffer_size);
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "%d},\n", config->output_indices[config->num_outputs-1]);
    CHECK_BUFFER(idx, buffer_size);

    idx += snprintf(&buffer[idx], buffer_size-idx, "\t.neurons = {\n");
    CHECK_BUFFER(idx, buffer_size);

    uint32_t offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        subneuron_description_t *neuron = (subneuron_description_t *)&config->neurons[offset];
        idx += snprintf(&buffer[idx], buffer_size-idx, "\t\t{.idx = %d, ", neuron->idx);
        CHECK_BUFFER(idx, buffer_size);
        idx += snprintf(&buffer[idx], buffer_size-idx, ".num_inputs = %d, ", neuron->num_inputs);
        CHECK_BUFFER(idx, buffer_size);
        idx += snprintf(&buffer[idx], buffer_size-idx, ".indices = {");
        CHECK_BUFFER(idx, buffer_size);
        for(uint32_t j=0; j<neuron->num_inputs-1; j++) {
            idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", neuron->indices[j]);
            CHECK_BUFFER(idx, buffer_size);
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "%d}}", neuron->indices[neuron->num_inputs-1]);
        CHECK_BUFFER(idx, buffer_size);
        if(i < config->num_neurons-1) {
            idx += snprintf(&buffer[idx], buffer_size-idx, ",");
            CHECK_BUFFER(idx, buffer_size);
        }
        idx += snprintf(&buffer[idx], buffer_size-idx, "\n");
        CHECK_BUFFER(idx, buffer_size);
        offset += 2 + neuron->num_inputs;
    }
    idx += snprintf(&buffer[idx], buffer_size-idx, "\t}\n};\n\n");
    CHECK_BUFFER(idx, buffer_size);
    return idx;
}

#define BUF_SIZE 100000

void micronet_save_data(micro_network_t * config, char *filename, char *prefix) {
    char buffer[BUF_SIZE] = {0};
    micronet_save_map(config, buffer, BUF_SIZE, prefix);
    
    write_buf_to_file(buffer, filename);

    // for(uint32_t i=0; i<config->num_neurons; i++) {
    //     uint32_t idx = snprintf(buffer, BUF_SIZE, "double %scoeffs_%04d[%d] = {\n", prefix, i, mini_neuron_get_num_coeffs(&config->neurons[i]));
    //     idx += mini_neuron_get_coeffs_as_string(&config->neurons[i], &buffer[idx], BUF_SIZE-idx);
    //     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\n};\n\n");
    //     write_buf_to_file(buffer, filename);
    // }
    // clear_buffer(buffer, BUF_SIZE);

    // uint32_t idx = 0;
    // idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double *%sbackup_coeffs[] = {\n", prefix);
    // for(uint32_t i=0; i<config->num_neurons-1; i++) {
    //     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d,\n", prefix, i);
    // }
    // idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d\n", prefix, config->num_neurons-1);
    // idx += snprintf(&buffer[idx], BUF_SIZE-idx, "};\n\n");
    // write_buf_to_file(buffer, filename);
}
