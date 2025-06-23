#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "utils.h"
#include "datasets.h"

int test_func(int(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name);
int test_func_no_ret(void(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name);
double get_error(neuron_params_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print);
