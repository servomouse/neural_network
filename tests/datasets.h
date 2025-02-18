#pragma once
#include <stdint.h>

typedef struct {
    double inputs[4];
    double output;
} dataset_entry_t;

typedef struct {
    double inputs[4];
    double output[1];
} u_dataset_entry_t;
