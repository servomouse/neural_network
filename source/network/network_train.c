#include "neuron_iface.h"
#include "neuron.h"

void network_clear_feedbacks(network_t *config); {
    for(uint32_t i=0; i<config->net_size; i++) {
        config->feedback_arr[i].value = 0.0;
        config->feedback_arr[i].stash = 0.0;
        config->feedback_arr[i].counter = 0;
        if(i>config->num_inputs) {
            neuron_clear_stashes(&config->neurons[i-config->num_inputs]);
        }
    }
}

void network_set_global_error(network_t *config, double error, double *output_errors) {
    for(uint32_t i=0; i<config->num_neurons; i++) {
        neuron_set_global_error(&config->neurons[i], error);
    }
    for(uint32_t i=0; i<config->num_outputs; i++) {
        config->feedback_arr[config->output_indices[i]].value = output_errors[i];
        config->feedback_arr[config->output_indices[i]].counter = 1;
    }
}

void network_update_feedbacks(network_t *config) {
    for(uint32_t i=config->net_size-1; i>config->num_inputs; i--) {
        neuron_generate_feedbacks(&config->neurons[i-config->num_inputs], config->feedback_arr, f_net, i);
    }
}

void network_update_coeffs(network_t *config) {
    for(uint32_t i=config->num_inputs; i<config->net_size; i++) {
        neuron_update_coeffs(&config->neurons[i], config->feedback_arr, c_net, i);
    }
}