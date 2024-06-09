#include "neuron.h"
#include <stdbool.h>
#include "utils.h"

#define RANDOM_CONNECT true

void init_neuron(neuron_t *neuron, neuron_t *net, uint32_t num_inputs, uint32_t idx) {
    if(num_inputs == 0) {
        neuron->num_inputs = 0;
        neuron->coeffitients = NULL;
        neuron->inputs = NULL;
        neuron->temp_output = 0;
        neuron->output = 0;
        return;
    }
    neuron->num_inputs = num_inputs;
    neuron->coeffitients = malloc(sizeof(double) * (1 << num_inputs));
    neuron->inputs = malloc(sizeof(uint32_t) * num_inputs);
    neuron->temp_output = 0;
    neuron->output = 0;
    if(RANDOM_CONNECT || (num_inputs < idx)) {
        for(uint32_t i=0; i<num_inputs; i++)
            neuron->inputs[i] = random_int(0, idx);
    } else {
        for(uint32_t i=0; i<num_inputs; i++)
            neuron->inputs[i] = idx-i;
    }
    for(uint32_t i=0; i<(1 << num_inputs); i++)
        neuron->coeffitients[i] = random_double(-1.0, 1.0);
}

void deinit_neuron(neuron_t *neuron) {
    if(neuron->coeffitients)
        free(neuron->coeffitients);
    if(neuron->inputs)
        free(neuron->inputs);
}

void calc_output(neuron_t *neuron, neuron_t *net) {
    return;
}
