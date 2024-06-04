#pragma once

#include "stdint.h"

typedef struct{
    uint32_t *inputs;
    uint32_t num_inputs;
    double *coeffitients;
    double output;
}neuron_t;

void init_neuron(neuron_t *neuron);
void calc_output(neuron_t *neuron, neuron_t *net);