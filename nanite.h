#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void init(uint32_t num_inputs);
void set_input_idx(uint32_t input_number, uint32_t input_idx);
void save_state(char *filename);
void restore_state(char *filename);
double get_output(double *inputs);
