#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    double *inputs;
    uint32_t *indices;
    struct {
        double offset;
        double amplitude;
    } *coeffs;
    // micro_network_t *micro_net;
    uint32_t num_coeffs;
    uint32_t num_inputs;
    double feedback_error;  // Error calculated by neurons connected to the output
    uint32_t feedback_error_count;
    double global_error;    // Error of the entire network
    uint32_t last_idx;
    struct {
        double offset;
        double amplitude;
    } last_value;
} neuron_params_t;

void init(uint32_t num_inputs);
void set_input_idx(uint32_t input_number, uint32_t input_idx);
void save_state(char *filename);
void restore_state(char *filename);
double get_output(double *inputs);
void set_feedback_error(double error);
void set_global_error(double error);
void mutate(void);
void restore(void);
void print_coeffs(void);
