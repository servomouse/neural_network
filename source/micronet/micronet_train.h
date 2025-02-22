#pragma once

#include "micronet.h"

void micronet_set_global_error(micro_network_t *config, double error, double *output_errors);
void micronet_update_feedbacks(micro_network_t *config, micro_network_t *f_net);
void micronet_clear_feedbacks(micro_network_t *config);
void micronet_update_coeffs(micro_network_t *config, micro_network_t *c_net);
