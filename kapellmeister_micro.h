#pragma once

#include "stdint.h"

typedef double(*get_output_func_t)(double*);

void init(uint32_t num_inputs, uint32_t net_size);
void set_output_function(uint32_t idx, get_output_func_t func);
void set_value(uint32_t idx, double value);
double get_value(uint32_t idx);
void tick_network(void);
void swap_arrays(void);
void save_state(char *filename);
void restore_state(char *filename);
void mutate(void);
void restore(void);
