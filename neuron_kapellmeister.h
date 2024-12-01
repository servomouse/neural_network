#pragma once

#include "stdint.h"

typedef double(*get_output_func_t)(double*);

void set_output_function(uint32_t idx, get_output_func_t func);
void init(uint32_t network_size);
