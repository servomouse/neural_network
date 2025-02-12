#pragma once
#include <stdint.h>

typedef struct {
    double inputs[4];
    double output;
} dataset_entry_t;

dataset_entry_t polynome_dataset[] = {
    {.inputs = {0.01, 0.05, 0.02, 0.03}, .output = 0.0},
    {.inputs = {0.07, 0.02, 0.05, 0.01}, .output = 0.0},
    {.inputs = {0.01, 0.05, 0.06, 0.04}, .output = 0.0},
    {.inputs = {0.02, 0.04, 0.07, 0.05}, .output = 0.0},
    {.inputs = {0.03, 0.03, 0.08, 0.06}, .output = 0.0},
    {.inputs = {0.04, 0.02, 0.09, 0.07}, .output = 0.0},
    {.inputs = {0.05, 0.01, 0.08, 0.98}, .output = 1.0},
    {.inputs = {0.06, 0.08, 0.97, 0.07}, .output = 1.0},
    {.inputs = {0.07, 0.07, 0.96, 0.96}, .output = 1.0},
    {.inputs = {0.08, 0.91, 0.05, 0.05}, .output = 1.0},
    {.inputs = {0.09, 0.92, 0.04, 0.95}, .output = 1.0},
    {.inputs = {0.01, 0.93, 0.93, 0.04}, .output = 1.0},
    {.inputs = {0.02, 0.94, 0.92, 0.93}, .output = 1.0},
    {.inputs = {0.91, 0.06, 0.01, 0.02}, .output = 1.0},
    {.inputs = {0.92, 0.05, 0.02, 0.91}, .output = 1.0},
    {.inputs = {0.93, 0.04, 0.93, 0.02}, .output = 1.0},
    {.inputs = {0.94, 0.03, 0.94, 0.93}, .output = 1.0},
    {.inputs = {0.95, 0.94, 0.05, 0.04}, .output = 1.0},
    {.inputs = {0.96, 0.95, 0.06, 0.95}, .output = 1.0},
    {.inputs = {0.97, 0.96, 0.97, 0.06}, .output = 1.0},
    {.inputs = {0.98, 0.97, 0.98, 0.97}, .output = 0.0},
    {.inputs = {0.97, 0.98, 0.99, 0.98}, .output = 0.0},
    {.inputs = {0.96, 0.97, 0.98, 0.97}, .output = 0.0},
    {.inputs = {0.95, 0.96, 0.97, 0.96}, .output = 0.0},
    {.inputs = {0.94, 0.95, 0.96, 0.95}, .output = 0.0},
    {.inputs = {0.93, 0.94, 0.95, 0.94}, .output = 0.0},
    {.inputs = {0.92, 0.93, 0.94, 0.93}, .output = 0.0},
    {.inputs = {0.91, 0.92, 0.93, 0.92}, .output = 0.0},
    {.inputs = {0.90, 0.91, 0.92, 0.91}, .output = 0.0},
};