#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void neuron_init(uint32_t num_inputs);
void neuron_set_input_idx(uint32_t input_number, uint32_t input_idx);
void neuron_save_state(char *filename);
void neuron_restore_state(char *filename);
double neuron_get_output(double *inputs);
void neuron_set_feedback_error(double error);
void neuron_set_global_error(double error);
void neuron_mutate(void);
void neuron_restore(void);
void neuron_print_coeffs(void);
