#include "neuron_iface.h"
#include "neuron.h"
#include "utils.h"

static void generate_part_feedback(neuron_params_t * n_params, double feedback_value, micro_network_t *feedback_micronet) {
    uint32_t stash_offset = MICRONET_STASH_SIZE;
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double micronet_inputs[5+MICRONET_STASH_SIZE] = {
            feedback_value,
            n_params->part_values[i],
            n_params->coeffs[i],
            n_params->output,
            n_params->global_error,
            n_params->coeffs_micronet_stash[stash_offset+0],
            n_params->coeffs_micronet_stash[stash_offset+1],
            n_params->coeffs_micronet_stash[stash_offset+2],
            n_params->feedback_micronet_stash[stash_offset+3],
            n_params->feedback_micronet_stash[stash_offset+4],
            n_params->feedback_micronet_stash[stash_offset+5],
        };
        // for(uint32_t j=0; j<MICRONET_STASH_SIZE; j++) {
        //     micronet_inputs[5+j] = n_params->feedback_micronet_stash[stash_offset+j];
        // };
        double *outputs = micronet_get_output(feedback_micronet, micronet_inputs);
        n_params->part_feedbacks[i] = outputs[0];
        for(uint32_t j=0; j<MICRONET_STASH_SIZE; j++) {
            n_params->feedback_micronet_stash[stash_offset+j] = outputs[j];
        };
        stash_offset += MICRONET_STASH_SIZE;
    }
}

static void generate_feedbacks_for_inputs(neuron_params_t * n_params) {
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        // uint32_t stash_offset = MICRONET_STASH_SIZE * i;
        double sum = 0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                sum += n_params->inputs[j];
            }
        }
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i)> 0) {
                double frac = n_params->inputs[j] / sum;
                n_params->inputs_feedback[j].value += frac * n_params->part_feedbacks[i];
                n_params->inputs_feedback[j].counter ++;
            }
        }
    }
}

void neuron_generate_feedbacks(neuron_params_t * n_params, complex_value_t *feedbacks, micro_network_t *feedback_micronet, uint32_t own_index) {
    // printf("neuron_generate_feedbacks() own index = %d\n", own_index);
    // printf("feedbacks[%d].counter = %d\n", own_index, feedbacks[own_index].counter);
    if(feedbacks[own_index].counter == 0) return;
    // Step 0: prepate arrays
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        n_params->part_feedbacks[i] = 0.0;
    }
    for(size_t i=1; i<n_params->num_inputs; i++) {
        n_params->inputs_feedback[i].value = 0.0;
        n_params->inputs_feedback[i].counter = 0;
    }
    // Step 1: generate part_feedback for each coeffitient
    generate_part_feedback(n_params, feedbacks[own_index].value/feedbacks[own_index].counter, feedback_micronet);

    // Step 2: calculate feedback for each input
    generate_feedbacks_for_inputs(n_params);

    // Step 3: set feedback values
    for(size_t i=0; i<n_params->num_inputs; i++) {
        double val;
        if(n_params->inputs_feedback[i].counter != 0) {
            val = n_params->inputs_feedback[i].value / n_params->inputs_feedback[i].counter;
        }
        feedbacks[n_params->indices[i]].value += val;
        feedbacks[n_params->indices[i]].counter ++;
        // printf("Feedback[%lld] value = %f\n", i, val);
    }
}

void neuron_update_coeffs(neuron_params_t * n_params, complex_value_t *feedbacks, micro_network_t *coeffs_micronet, uint32_t own_index) {
    // printf("neuron_update_coeffs() own index = %d\n", own_index);
    // printf("feedbacks[%d].counter = %d\n", own_index, feedbacks[own_index].counter);
    if(feedbacks[own_index].counter == 0) return;

    uint32_t stash_offset = 0.0;
    for(size_t i=0; i<n_params->num_coeffs; i++) {
        double micronet_inputs[5+MICRONET_STASH_SIZE] = {
            feedbacks[own_index].value/feedbacks[own_index].counter,
            n_params->part_values[i],
            n_params->coeffs[i],
            n_params->output,
            n_params->global_error,
            n_params->feedback_micronet_stash[stash_offset+0],
            n_params->feedback_micronet_stash[stash_offset+1],
            n_params->feedback_micronet_stash[stash_offset+2],
            n_params->coeffs_micronet_stash[stash_offset+3],
            n_params->coeffs_micronet_stash[stash_offset+4],
            n_params->coeffs_micronet_stash[stash_offset+5],
        };
        // for(uint32_t j=0; j<MICRONET_STASH_SIZE; j++) {
        //     micronet_inputs[5+j] = n_params->coeffs_micronet_stash[stash_offset+j];
        // };
        double *outputs = micronet_get_output(coeffs_micronet, micronet_inputs);
        for(uint32_t j=0; j<MICRONET_STASH_SIZE; j++) {
            n_params->coeffs_micronet_stash[stash_offset+j] = outputs[j];
        };
        n_params->coeffs[i] += outputs[0];
        
        if(n_params->coeffs[i] != n_params->coeffs[i]) {
            printf("Coeffs update error! Coeffs: ");
            for(uint32_t i=0; i<n_params->num_coeffs; i++) {
                printf("%f, ", n_params->coeffs[i]);
            }
            printf("\noutputs[0] = %f\n", outputs[0]);
            exit(0);
        }
        // printf("Coeff delta = %f\n", outputs[0]);
        stash_offset += MICRONET_STASH_SIZE;
    }
    feedbacks[own_index].value = 0.0;
    feedbacks[own_index].counter = 0;
}
