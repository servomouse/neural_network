#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

void neuron_init_coeffs(neuron_params_t *n) {
    double coeffs[5] = {-0.02, -0.01, 0.01, 0.02, 0.03};

    for(uint32_t i=0; i<5; i++) {
        neuron_set_coeff(n, i, coeffs[i]);
    }
}

int test_mutation(neuron_params_t *n) {
    neuron_init_coeffs(n);

    neuron_mutate(n);
    double error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(error == 0) {
        printf("Error: mutation doesn't work, error after mutation == 0");
        exit(EXIT_FAILURE);
    }
    neuron_rollback(n);
    error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(error != 0) {
        printf("Error: rollback after mutation doesn't work, error after rollback != 0");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int test_rollback(neuron_params_t *n) {
    neuron_init_coeffs(n);
    double init_error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);

    neuron_stash_state(n);

    double error;
    for(uint32_t i=0; i<5; i++) {
        neuron_set_coeff(n, i, 0.5);
        error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
        if(error == init_error) {
            printf("Test rollback error: changing coeffitient does not affect neuron output!");
            exit(EXIT_FAILURE);
        }
        neuron_rollback(n);
    }
    error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(error != 0) {
        printf("Error: rollback doesn't work, error after rollback != 0");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int test_save_restore(neuron_params_t *n) {
    neuron_init_coeffs(n);

    compressed_neuron_t *c_neuron = neuron_save(n);
    neuron_set_coeff(n, 0, 0.5);
    neuron_set_coeff(n, 1, 0.5);
    neuron_set_coeff(n, 2, 0.5);
    neuron_set_coeff(n, 3, 0.5);
    double error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(0 == error) {
        printf("Save-restore error: error after modification == 0!\n");
        exit(EXIT_FAILURE);
    }

    neuron_restore(n, c_neuron);
    free(c_neuron);

    error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(error != 0) {
        printf("Error: save-restore doesn't work: error after restore != 0");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int test_output(neuron_params_t *n) {
    neuron_init_coeffs(n);

    // Test initial error:
    double error = get_error(n, linear_dataset, sizeof_arr(linear_dataset), 0);
    if(error != 0) {
        printf("Error: initial error != 0! Initial error: %f", error);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int main() {
    srand(time(NULL));
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_output,       &linear_neuron, "linear neuron output")     ||
       test_func(test_rollback,     &linear_neuron, "linear neuron rollback")   ||
       test_func(test_mutation,     &linear_neuron, "linear neuron mutations")  ||
       test_func(test_save_restore, &linear_neuron, "linear neuron save-restore")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}