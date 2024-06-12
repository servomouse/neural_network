#include "neuron.h"
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define RANDOM_CONNECT true

double limit_value(double value) {
    if(value > 1.0)
        return 1.0;
    if(value < -1.0)
        return -1.0;
    return value;
}

void init_neuron(neuron_t *neuron, neuron_t *net, uint32_t num_inputs, uint32_t idx) {
    // neuron = malloc(sizeof(neuron_t));
    neuron->num_inputs = num_inputs;
    neuron->num_coeffitients = 1 << num_inputs;
    neuron->idx = idx;
    neuron->temp_output = 0;
    neuron->output = 0;
    neuron->coeffitients = NULL;
    neuron->inputs = NULL;
    if(num_inputs == 0) return;

    neuron->coeffitients = malloc(sizeof(double) * (neuron->num_coeffitients));
    neuron->inputs = malloc(num_inputs * sizeof(uint32_t));
    if(RANDOM_CONNECT || (num_inputs < idx)) {
        for(uint32_t i=0; i<num_inputs; i++)
            neuron->inputs[i] = random_int(0, idx);
    } else {
        for(uint32_t i=0; i<num_inputs; i++)
            neuron->inputs[i] = idx-i;
    }
    for(uint32_t i=0; i<(neuron->num_coeffitients); i++)
        neuron->coeffitients[i] = random_double(-0.1, 0.1);
}

void deinit_neuron(neuron_t *neuron) {
    if(neuron->coeffitients)
        free(neuron->coeffitients);
    if(neuron->inputs)
        free(neuron->inputs);
}

void calc_output(neuron_t *neuron, neuron_t *net) {
    if(neuron->num_inputs == 0)
        return;
    double temp_output = neuron->coeffitients[0];
    for(uint32_t i=0; i<(neuron->num_coeffitients); i++) {
        double temp = neuron->coeffitients[i];
        for(uint32_t j=0; j<neuron->num_inputs; j++) {
            if(((1 << j) & i) > 0)
                temp *= net[neuron->inputs[j]].output;
        }
        temp_output += temp;
    }
    neuron->temp_output = limit_value(temp_output);
}

uint32_t update_output(neuron_t *neuron) {
    uint32_t changed = (neuron->output == neuron->temp_output)? 1:0;
    neuron->output = neuron->temp_output;
    return changed;
}

void random_mutation(neuron_t *neuron) {
    // printf("neuron: %d\n", neuron);
    uint32_t idx = random_int(0, neuron->num_coeffitients);
    double delta = random_double(-0.0001, 0.0001);
    neuron->old_coeffitient.idx = idx;
    neuron->old_coeffitient.value = neuron->coeffitients[idx];
    neuron->coeffitients[idx] += delta;
}

void rollback_mutation(neuron_t *neuron) {
    uint32_t idx = neuron->old_coeffitient.idx;
    neuron->coeffitients[idx] = neuron->old_coeffitient.value;
}
