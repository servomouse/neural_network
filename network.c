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

typedef struct {
    uint32_t idx;
    uint8_t field_size;
    double value;
} mystruct_t;

    uint32_t net_size;
    uint32_t num_inputs;
    uint32_t num_outputs;

void save_network(network_t *s, char *filename) {
    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        perror("Error opening file");
        return;
    }
    fprint(file, "{\n");
    fprint(file, "'net_size': %u, \n", s->net_size);
    fprint(file, "'num_inputs': %u, \n", s->num_inputs);
    fprint(file, "'num_outputs': %u, \n", s->num_outputs);
    // fprint(file, "'value': %.2f\n", s->value);
    fprint(file, "}\n");
    fclose(file);
}

int restore_network(network_t *s, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file");
        return;
    }
    // int res = fscanf(file, "{\n 'value': %%lf\n}\n", &s->value);
    int res = fscanf(file, "{\n 'net_size': %u,\n'num_inputs': %hhu'\n'num_outputs': %%lf\n}\n", &s->net_size, &s->num_inputs, &s->num_outputs);
    fclose(file);
    if(res == 3) {
        return 0;   // Success
    } else {
        return -1;  // Error
    }
}

void test_save_restore(void) {
    // Initialize random number generator
    srand((unsigned int)time(NULL));

    // Create and populate the structure with random values
    network_t originalData;
    originalData.net_size = rand();
    originalData.num_inputs = rand() % 256;
    originalData.num_outputs = rand() % 256;
    // originalData.value = (double)rand() / RAND_MAX * 100.0; // Random double value between 0 and 100

    save_network(&originalData, "test_data.json");

    network_t newData;
    restore_network(&newData, "test_data.json");

    // Compare the values
    if(originalData.net_size == newData.net_size &&
       originalData.num_inputs == newData.num_inputs &&
       originalData.num_outputs == newData.num_outputs) {
        printf("Success: The values match!\n");
    } else {
        printf("Error: The values do not match!\n");
    }
}

void print_results(network_t *net) {
    print_coeffs(&net->neurons[4], net->neurons);
}
