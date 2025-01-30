#include "neuron_complex_values.h"

double neuron_get_complex_item_value(complex_item_t *item) {
    if(item->counter > 0)
        return item->tot_value/item->counter;
    return 0;
}

double neuron_get_complex_item_value_mod(complex_item_t *item) {
    if(item->counter > 0)
        return item->tot_value_mod/item->counter;
    return 0;
}

void neuron_sub_complex_item_value(complex_item_t *item, double value) {
    item->tot_value -= value;
    if(value < 0)
        item->tot_value_mod += value;
    else
        item->tot_value_mod -= value;
    if(item->counter > 0)
        item->counter --;
}

void neuron_add_complex_item_value(complex_item_t *item, double value) {
    if(value < item->min_value) {
        item->min_value = value;
    }
    if(value > item->max_value) {
        item->max_value = value;
    }
    item->tot_value += value;
    if(value < 0)
        item->tot_value_mod -= value;
    else
        item->tot_value_mod += value;
    item->counter ++;
    
}

void neuron_clear_complex_items(complex_item_t *items, uint32_t num_elements) {
    for(uint32_t i=0; i<num_elements; i++) {
        items[i].min_value = 0;
        items[i].max_value = 0;
        items[i].tot_value = 0;
        items[i].tot_value_mod = 0;
        items[i].counter = 0;
    }
}