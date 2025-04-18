#pragma once
// python new_build.py controller
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "neuron_types.h"

#define MICRONET_STASH_SIZE 6

void neuron_init(neuron_params_t * n_params, neuron_type_t n_type, uint32_t num_inputs);
void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx);
void neuron_set_coeff(neuron_params_t * n_params, uint32_t idx, double new_value);
void neuron_save_state(neuron_params_t * n_params, char *filename);
void neuron_set_coeffs(neuron_params_t * n_params, double *coeffs);
double neuron_get_coeff(neuron_params_t * n_params, uint32_t idx);
int neuron_get_coeffs_as_string(neuron_params_t *n_params, char *buffer, uint32_t buffer_size);

// Old version
void neuron_save(neuron_params_t * n_params, char *filename);
void neuron_restore(neuron_params_t * n_params, char *filename);

// New version
uint32_t neuron_get_data_size(neuron_params_t *neuron);
compressed_neuron_t * neuron_save_new(neuron_params_t * n_params);
uint32_t neuron_restore_new(neuron_params_t * n_params, compressed_neuron_t * n_data);

void neuron_stash_state(neuron_params_t * n_params);
void neuron_mutate(neuron_params_t * n_params);
void neuron_rollback(neuron_params_t * n_params);

void neuron_save_coeffs(neuron_params_t * n_params, uint32_t neuron_idx);
void neuron_prepare_file(const char *filename);
void neuron_save_data(neuron_params_t * n_params, const char *filename, uint32_t neuron_idx);
void neuron_complete_file(const char *filename, uint32_t num_neurons);
void neuron_restore_data(neuron_params_t * n_params, uint32_t neuron_idx);

void neuron_restore_state(neuron_params_t * n_params, char *filename);
double neuron_get_output(neuron_params_t * n_params, double *inputs);
void neuron_set_feedback_error(neuron_params_t * n_params, double error);
void neuron_reset_feedback_error(neuron_params_t * n_params);
void neuron_clear_stashes(neuron_params_t * n_params);
void neuron_set_global_error(neuron_params_t * n_params, double error);


void neuron_backup(neuron_params_t *n_params);
void neuron_check_backup(neuron_params_t *n_params);

void neuron_print_coeffs(neuron_params_t * n_params);
uint32_t neuron_get_num_outputs(neuron_params_t * n_params);
uint32_t neuron_get_num_coeffs(neuron_params_t * n_params);
void neuron_set_num_outputs(neuron_params_t * n_params, uint32_t new_value);
// void neuron_set_output_idx(neuron_params_t * n_params, uint32_t output_idx);
uint32_t neuron_get_output_idx(neuron_params_t * n_params);
void neuron_reset_output_counter(neuron_params_t * n_params);
