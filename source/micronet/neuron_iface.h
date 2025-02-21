#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"
#include "micronet.h"

void neuron_generate_feedbacks(neuron_params_t * n_params, feedback_item_t *feedbacks, micro_network_t *f_micronet, uint32_t own_index);
void neuron_update_coeffs(neuron_params_t * n_params, feedback_item_t *feedbacks, micro_network_t *c_micronet, uint32_t own_index);
