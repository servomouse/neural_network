#include "network.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define INPUTS_NUM 4

network_t* create_network(uint32_t num_inputs, uint32_t num_neurons, uint32_t num_outputs) {
    if(num_outputs > num_neurons) {
        printf("num_neurons cannot be less than num_outputs");
        return NULL;
    }
    network_t *net = malloc(sizeof(network_t));
    net->last_mutant_idx = 0;
    net->num_inputs = num_inputs;
    net->num_outputs = num_outputs;
    net->net_size = net->num_inputs + num_neurons;
    net->neurons = malloc(sizeof(neuron_t) * net->net_size);
    for(uint32_t i=0; i<(net->net_size); i++) {
        if(i < net->num_inputs)
            init_neuron(&net->neurons[i], net->neurons, 0, i);
        else
            init_neuron(&net->neurons[i], net->neurons, INPUTS_NUM, i);
    }

    return net;
}

double get_output(network_t *net, uint32_t num_inputs, double inputs[]) {
    for(uint32_t i=0; i<num_inputs; i++) {
        net->neurons[i].output = inputs[i];
    }
    for(uint32_t i=0; i<(net->net_size); i++) {
        calc_output(&net->neurons[i], net->neurons);
        update_output(&net->neurons[i]);
    }
    return net->neurons[net->net_size-1].output;
}

void mutate(network_t *net) {
    net->last_mutant_idx = random_int(net->num_inputs, net->net_size);
    random_mutation(&net->neurons[net->last_mutant_idx]);
}

void repair(network_t *net) {
    rollback_mutation(&net->neurons[net->last_mutant_idx]);
}

// neurons = {
//     0: {
//         "num_inputs": 5,
//         "num_coeffs": 32,
//         "inputs": [0, 1, 2, 3, 4],
//         "coeffs": {0.1, 0.2, 0.3 . . . ]
//     }
// }

void save_network(network_t *s, char *filename) {
    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "{\n");
    fprintf(file, "\t\"net_size\": %u,\n", s->net_size);
    fprintf(file, "\t\"num_inputs\": %u,\n", s->num_inputs);
    fprintf(file, "\t\"num_outputs\": %u,\n", s->num_outputs);
    fprintf(file, "\t\"neurons\": {\n");
    for(uint32_t i=0; i<s->net_size; i++) {
        fprintf(file, "\t\t\"%u\": {\n", i);
        fprintf(file, "\t\t\"num_inputs\": %u,\n", s->neurons[i].num_inputs);
        fprintf(file, "\t\t\"inputs\": [");
        for(uint32_t j=0; j<s->neurons[i].num_inputs; j++) {
            fprintf(file, "%u, ", s->neurons[i].inputs[j]);
        }
        fprintf(file, "],\n");
        fprintf(file, "\t\t\"num_coeffs\": %u,\n", s->neurons[i].num_coeffitients);
        fprintf(file, "\t\t\"coeffs\": [");
        for(uint32_t j=0; j<s->neurons[i].num_coeffitients; j++) {
            fprintf(file, "%.2f, ", s->neurons[i].coeffitients[j]);
        }
        fprintf(file, "]\n");
        fprintf(file, "\t},\n");
    }
    fprintf(file, "\t}\n");
    // fprint(file, "'value': %.2f\n", s->value);
    fprintf(file, "}\n");
    fclose(file);
}

int restore_network(network_t *s, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file");
        return -1;
    }
    // int res = fscanf(file, "{\n 'value': %%lf\n}\n", &s->value);
    int res = fscanf(file, "{\n\t\"net_size\": %u,\n\t\"num_inputs\": %u,\n\t\"num_outputs\": %u\n}\n",
                     &s->net_size, &s->num_inputs, &s->num_outputs);
    fclose(file);
    if(res == 3) {
        return 0;   // Success
    } else {
        return -1;  // Error
    }
}

int test_save_restore_network(char *filename) {
    network_t original_data;
    original_data.net_size = rand();
    original_data.num_inputs = rand() % 256;
    original_data.num_outputs = rand() % 256;
    // originalData.value = (double)rand() / RAND_MAX * 100.0; // Random double value between 0 and 100

    save_network(&original_data, filename);

    network_t new_data;
    restore_network(&new_data, filename);

    printf("orig net_size: %u, read value: %u\n", original_data.net_size, new_data.net_size);
    printf("orig num_inputs: %u, read value: %u\n", original_data.num_inputs, new_data.num_inputs);
    printf("orig num_outputs: %u, read value: %u\n", original_data.num_outputs, new_data.num_outputs);

    if((original_data.net_size != new_data.net_size) &&
       (original_data.num_inputs != new_data.num_inputs) &&
       (original_data.num_outputs != new_data.num_outputs)) {
        printf("Error: The values of num_outputs do not match!\n");
        return -1;
    }
    remove(filename);
    printf("Success: The values match!\n");
    return 0;
}

void print_results(network_t *net) {
    print_coeffs(&net->neurons[4], net->neurons);
}
