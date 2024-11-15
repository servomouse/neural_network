#include "neuron_header.h"
#include <stdlib.h>

// Neuron of the basic type 0

typedef struct{
    double *inputs;
    uint16_t *inputs_indices;
    uint8_t inputs_num;
    double *coeffs;
    uint16_t coeffs_num;
} _config_t;

_config_t *_config;

void init_neuron(config_t *config) {
    _config = calloc(1, sizeof(_config_t));
    _config->inputs = config->inputs;
    _config->inputs_indices = config->inputs_indices;
    _config->inputs_num = config->inputs_num;
    _config->coeffs = config->coeffs;
    _config->coeffs_num = config->coeffs_num;
}

double calc_output(void) {
    double ret_val = _config->coeffs[0];
    for(int i=0; i<_config->coeffs_num; i++) {
        for(int j=0; j<_config->inputs_num; j++) {
            if(((1 << j) & i) > 0) {
                ret_val += _config->inputs[_config->inputs_indices[j]] * _config->coeffs[i];
            }
        }
    }
    return ret_val;
}
