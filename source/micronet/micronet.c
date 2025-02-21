#include <stdlib.h>
#include "micronet.h"
#include "neuron.h"
#include "neuron_iface.h"
#include "utils.h"
#include <time.h>

// Inputs:
//          - global error
//          - local error
//          - input(s) value for coefficient and coeffs values for input feedback
//          - previous target value
//          - neuron output
//          - sum of other coeffs * input values

void micronet_init(micro_network_t * config, micronet_map_t *net_map, double **bckp_coeffs) {
    config->map = net_map;
    config->map->neurons = &net_map->neurons[0];
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

    config->arr          = calloc(config->net_size, sizeof(double));
    config->feedback_arr = calloc(config->net_size, sizeof(feedback_item_t));
    config->neurons      = calloc(config->num_neurons, sizeof(neuron_params_t));

    uint32_t offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        subneuron_description_t *neuron = (subneuron_description_t *)&net_map->neurons[offset];
        uint32_t idx        = neuron->idx - config->num_inputs;
        uint32_t num_inputs = neuron->num_inputs;
        uint32_t n_type = neuron->n_type;
        // printf("Neuron %d: idx = %d, num_inputs = %d, offset = %d\n", i, idx, num_inputs, offset);
        neuron_init(&config->neurons[idx], n_type, num_inputs);
        for(size_t j=0; j<num_inputs; j++) {
            neuron_set_input_idx(&config->neurons[idx], j, neuron->indices[j]);
        }
        if(bckp_coeffs) {
            neuron_set_coeffs(&config->neurons[idx], bckp_coeffs[idx]);
        }
        offset += 3 + num_inputs;
    }
}

double *micronet_get_output(micro_network_t * config, double *inputs) {
    // for(int i=0; i<config->num_neurons; i++) {
    //     neuron_reset_output_counter(&config->neurons[i]);
    // }
    // printf("Micronet array: ");
    for(uint32_t i=0; i<config->net_size; i++) {
        if(i < config->num_inputs) {
            config->arr[i] = inputs[i];
        } else {
            config->arr[i] = neuron_get_output(&config->neurons[i-config->num_inputs], config->arr);
        }
        // printf("%f, ", config->arr[i]);
    }
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->outputs[i] = config->arr[config->output_indices[i]];
    }
    if(config->outputs[0] != config->outputs[0]) {
        printf("Micornet error! array:\n");
        for(uint32_t i=0; i<config->net_size; i++) {
            printf("%f, ", config->arr[i]);
        }
        printf("\n");
        printf("Inputs:\n");
        for(uint32_t i=0; i<config->num_inputs; i++) {
            printf("%f, ", inputs[i]);
        }
        printf("\n");
        exit(0);
    }
    // printf("\n");
    return config->outputs;
}

void micronet_set_global_error(micro_network_t *config, double error) {
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_set_global_error(&config->neurons[i-config->num_inputs], error);
    }
}

void micronet_update_feedbacks(micro_network_t *config, micro_network_t *f_net) {
    for(uint32_t i=config->net_size-1; i>config->num_inputs; i++) {
        neuron_generate_feedbacks(&config->neurons[i-config->num_inputs], config->feedback_arr, f_net, i);
    }
}

void micronet_clear_feedbacks(micro_network_t *config) {
    for(uint32_t i=0; i<config->net_size; i++) {
        config->feedback_arr[i].value = 0.0;
        config->feedback_arr[i].stash = 0.0;
        config->feedback_arr[i].counter = 0;
        if(i>config->num_inputs) {
            neuron_clear_stashes(&config->neurons[i-config->num_inputs]);
        }
    }
}

void micronet_update_coeffs(micro_network_t *config, micro_network_t *c_net) {
    for(uint32_t i=config->net_size-1; i>config->num_inputs; i++) {
        neuron_update_coeffs(&config->neurons[i], config->feedback_arr, c_net, i);
    }
}

// void micronet_save(micro_network_t * config, char *filename) {
//     char *arr_fname = concat_strings("arr_", filename);
//     store_data(&config, sizeof(config), filename);
//     store_data(&config->arr, config->net_size * sizeof(double), arr_fname);
//     free(arr_fname);
// }

// void micronet_restore(micro_network_t * config, char *filename) {
//     if(!config->neurons) {
//         printf("Call init() before calling restore_state()!");
//         exit(1);
//     }
//     char *arr_fname = concat_strings("arr_", filename);
//     restore_data(&config, sizeof(config), filename);
//     restore_data(config->arr, config->net_size * sizeof(double), arr_fname);
//     free(arr_fname);
// }

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
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_print_coeffs(&config->neurons[i-config->num_inputs]);
    }
}

#define CHECK_BUFFER(_idx, _buf_size) {if(_idx >= (_buf_size-1)) {printf("ERROR: Buffer is too small! Exit\n"); exit(-1);}}

// // Buffer is supposed to be empty, returns number of bytes written to the buffer
// static int micronet_save_map(micro_network_t * config, char *buffer, uint32_t buffer_size, char *prefix, char *filename) {
//     // uint32_t num_inputs;
//     // uint32_t num_neurons;
//     // uint32_t net_size;
//     // uint32_t *neurons;  // subneuron_description_t is used here
//     // uint32_t num_outputs;
//     // uint32_t output_indices[];

//     uint32_t idx = 0;

//     idx += snprintf(&buffer[idx], buffer_size-idx, "uint32_t %sneurons[] = {\n", prefix);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t//\tidx\t\tnum_inputs\tindices\n");
//     uint32_t offset = 0;
//     for(uint32_t i=0; i<config->num_neurons; i++) {

//         subneuron_description_t *neuron = (subneuron_description_t *)&config->map->neurons[offset];
//         idx += snprintf(&buffer[idx], buffer_size-idx, "\t\t%d,\t\t", neuron->idx);
//         idx += snprintf(&buffer[idx], buffer_size-idx, "%d,\t\t\t", neuron->num_inputs);
//         idx += snprintf(&buffer[idx], buffer_size-idx, "%d,\t\t\t", neuron->n_type);
//         // idx += snprintf(&buffer[idx], buffer_size-idx, ".indices = {");

//         for(uint32_t j=0; j<neuron->num_inputs-1; j++) {
//             idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", neuron->indices[j]);
//         }
//         idx += snprintf(&buffer[idx], buffer_size-idx, "%d", neuron->indices[neuron->num_inputs-1]);

//         if(i < config->num_neurons-1) {
//             idx += snprintf(&buffer[idx], buffer_size-idx, ",");
//         }
//         idx += snprintf(&buffer[idx], buffer_size-idx, "\n");
//         offset += 3 + neuron->num_inputs;
//     }
//     idx += snprintf(&buffer[idx], buffer_size-idx, "};\n\n");

//     // idx += snprintf(&buffer[idx], buffer_size-idx, "#pragma once\n");
//     // idx += snprintf(&buffer[idx], buffer_size-idx, "#include \"neuron_types.h\"\n\n");
//     idx += snprintf(&buffer[idx], buffer_size-idx, "micronet_map_t %sbackup_map = {\n", prefix);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_inputs = %d,\n", config->num_inputs);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_neurons = %d,\n", config->num_neurons);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.net_size = %d,\n", config->net_size);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.neurons = %sneurons,\n", prefix);
//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.num_outputs = %d,\n", config->num_outputs);

//     idx += snprintf(&buffer[idx], buffer_size-idx, "\t.output_indices = {");
//     for(uint32_t i=0; i<config->num_outputs-1; i++) {
//         idx += snprintf(&buffer[idx], buffer_size-idx, "%d, ", config->output_indices[i]);
//     }
//     idx += snprintf(&buffer[idx], buffer_size-idx, "%d}\n", config->output_indices[config->num_outputs-1]);

//     idx += snprintf(&buffer[idx], buffer_size-idx, "};\n\n");
//     return idx;
// }

// #define BUF_SIZE 8192

// void micronet_save_data(micro_network_t * config, char *filename, char *prefix, char *to_define) {
//     char buffer[BUF_SIZE] = {0};
//     uint32_t idx = 0;

//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "#pragma once\n");
//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "#include \"neuron_types.h\"\n\n");

//     write_buf_to_file(buffer, filename);
//     micronet_save_map(config, buffer, BUF_SIZE, prefix, filename);
    
//     write_buf_to_file(buffer, filename);
//     clear_buffer(buffer, BUF_SIZE);
//     idx = 0;

//     for(uint32_t i=0; i<config->num_neurons; i++) {
//         idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double %scoeffs_%04d[%d] = {\n", prefix, i, neuron_get_num_coeffs(&config->neurons[i]));
//         idx += neuron_get_coeffs_as_string(&config->neurons[i], &buffer[idx], BUF_SIZE-idx);
//         idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\n};\n\n");

//         write_buf_to_file(buffer, filename);
//         clear_buffer(buffer, BUF_SIZE);
//         idx = 0;
//     }

//     // uint32_t idx = 0;
//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "double *%sbackup_coeffs[] = {\n", prefix);
//     for(uint32_t i=0; i<config->num_neurons-1; i++) {
//         idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d,\n", prefix, i);
//     }
//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "\t%scoeffs_%04d\n", prefix, config->num_neurons-1);
//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "};\n\n");
//     idx += snprintf(&buffer[idx], BUF_SIZE-idx, "#define %s\n", to_define);
//     write_buf_to_file(buffer, filename);
// }
