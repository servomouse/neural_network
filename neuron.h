#pragma once

#include "stdint.h"

typedef struct {
    uint32_t index;
    uint32_t id;
} neuron_input_t;

typedef struct{
    neuron_input_t *inputs;
    // uint32_t *inputs;
    uint32_t num_inputs;
    uint32_t idx;
    double *coeffitients;
    uint32_t num_coeffitients;
    struct {
        uint32_t idx;
        double value;
    } old_coeffitient;
    double temp_output;
    double output;
}neuron_t;

void init_neuron(neuron_t *neuron, neuron_t *net, uint32_t num_inputs, uint32_t idx);
void deinit_neuron(neuron_t *neuron);
void calc_output(neuron_t *neuron, neuron_t *net);
uint32_t update_output(neuron_t *neuron);
void random_mutation(neuron_t *neuron);
void rollback_mutation(neuron_t *neuron);
void print_coeffs(neuron_t *neuron, neuron_t *net);
