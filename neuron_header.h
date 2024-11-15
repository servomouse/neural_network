#pragma once

#include "stdint.h"

typedef struct{
    double   *inputs;
    uint16_t *inputs_indices;
    uint8_t   inputs_num;
    double   *coeffs;
    uint16_t  coeffs_num;
} config_t;

void init_neuron(config_t *config);
void calc_output(void);