#pragma once

#include "stdint.h"

typedef struct{
    double   *inputs;
    uint16_t *inputs_indices;
    uint8_t   inputs_num;
    double   *coeffs;
    uint16_t  coeffs_num;
} config_t;

#ifdef __unix__
    #define ABI_FUNCTION 
#elif defined(_WIN32) || defined(WIN32)
    #define ABI_FUNCTION __declspec(dllexport)
#endif

void init_neuron(config_t *config);
double get_output(void);
void set_output(double value);
void set_inputs(double *inputs);