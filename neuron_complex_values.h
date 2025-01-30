#pragma once

#include "neuron.h"

void neuron_add_complex_item_value(complex_item_t *item, double value);
void neuron_clear_complex_items(complex_item_t *items, uint32_t num_elements);
double neuron_get_complex_item_value(complex_item_t *item);
void neuron_sub_complex_item_value(complex_item_t *item, double value);
