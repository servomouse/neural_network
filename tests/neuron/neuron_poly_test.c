#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

#define DATASET polynome_dataset
#define DATASET_SIZE sizeof_arr(DATASET)

void test_mutation(neuron_params_t *n) {
    double init_error = get_error(n, DATASET, DATASET_SIZE, 0);

    neuron_mutate(n);
    double error = get_error(n, DATASET, DATASET_SIZE, 0);
    if(error == init_error) {
        RAISE("Error: mutation doesn't work, error == init_error");
    }
}

void test_rollback(neuron_params_t *n) {
    double init_error = get_error(n, DATASET, DATASET_SIZE, 0);
    neuron_stash_state(n);

    double error;
    double coeff_values[] = {0.5, 1.0};
    for(uint32_t i=0; i<4; i++) {
        neuron_set_coeff(n, i, (void*)&coeff_values);
        error = get_error(n, DATASET, DATASET_SIZE, 0);
        neuron_rollback(n);
    }
    error = get_error(n, DATASET, DATASET_SIZE, 0);
    if(error != init_error) {
        RAISE("Error: rollback doesn't work, error != init_error");
    }
}

void test_save_restore(neuron_params_t *n) {
    double init_error = get_error(n, DATASET, DATASET_SIZE, 0);
    compressed_neuron_t *c_neuron = neuron_save(n);
    double coeff_values[] = {0.5, 1.0};
    for(uint32_t i=0; i<4; i++) {
        neuron_set_coeff(n, i, (void*)&coeff_values);
    }
    neuron_restore(n, c_neuron);
    free(c_neuron);
    double error = get_error(n, DATASET, DATASET_SIZE, 0);
    if(init_error != error) {
        RAISE("Error: save-restore doesn't work: error after restore != error before save");
    }
}

void test_output(neuron_params_t *n) {
    neuron_stash_state(n);
    for(uint32_t i=0; i<16; i++) {
        double coeff_values[] = {0.02 * i+1, 1.0};
        neuron_set_coeff(n, i, (void*)&coeff_values);
    }
    double error = get_error(n, DATASET, DATASET_SIZE, 0);
    neuron_rollback(n);
    if(!are_equal(error, 0.517241, 6)) {
        RAISE("Error: error value differs from expected! Value: %f\n", round_to_precision(error, 6));
    }
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func_no_ret(test_output,       &poly_neuron, "poly neuron output")     ||
       test_func_no_ret(test_mutation,     &poly_neuron, "poly neuron mutations")  ||
       test_func_no_ret(test_rollback,     &poly_neuron, "poly neuron rollback")   ||
       test_func_no_ret(test_save_restore, &poly_neuron, "poly neuron save-restore")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}