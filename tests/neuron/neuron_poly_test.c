#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

int test_mutation(neuron_params_t *n) {
    double init_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error before mutation = %f;\n", init_error);

    neuron_mutate(n);
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error after mutation = %f;\n", error);
    if(error == init_error) {
        printf("Error: mutation doesn't work, error == init_error");
        return EXIT_FAILURE;
    }

    // neuron_rollback(n);
    // error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error after mutation = %f;\n", error);
    // if(error != init_error) {
    //     printf("Error: rollback doesn't work, error != init_error");
    //     return EXIT_FAILURE;
    // }
    return EXIT_SUCCESS;
}

int test_rollback(neuron_params_t *n) {
    double init_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error before mutation = %f;\n", init_error);
    neuron_stash_state(n);

    double error;
    for(uint32_t i=0; i<4; i++) {
        neuron_set_coeff(n, i, 0.5);
        error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
        // printf("Error after setting coeff %d = %f;\n", i, error);
        neuron_rollback(n);
    }
    error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error after rollback = %f;\n", error);
    if(error != init_error) {
        printf("Error: rollback doesn't work, error != init_error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_save_restore(neuron_params_t *n) {
    double init_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    compressed_neuron_t *c_neuron = neuron_save(n);
    neuron_set_coeff(n, 0, 0.5);
    neuron_set_coeff(n, 1, 0.5);
    neuron_set_coeff(n, 2, 0.5);
    neuron_set_coeff(n, 3, 0.5);
    // double init_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // printf("Error after setting coeffs = %f;\n", error);
    neuron_restore(n, c_neuron);
    free(c_neuron);
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    if(init_error != error) {
        printf("Error: save-restore doesn't work: error after restore != error before save");
        return EXIT_FAILURE;
    }
    // printf("Error after restore = %f;\n", error);
    return EXIT_SUCCESS;
}

int test_output(neuron_params_t *n) {
    neuron_stash_state(n);
    for(uint32_t i=0; i<16; i++) {
        // printf("Setting coeff[%d] to %f\n", i, 0.02 * i);
        neuron_set_coeff(n, i, 0.02 * i+1);
    }
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    neuron_rollback(n);
    // printf("Error after setting coeffs = %f;\n", error);
    if(round_to_precision(error, 6) == 0.517241) {
        return EXIT_SUCCESS;
    }
    printf("Error: error value differs from expected!");
    return EXIT_FAILURE;
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func(test_output,       &poly_neuron, "poly neuron output")     ||
       test_func(test_mutation,     &poly_neuron, "poly neuron mutations")  ||
       test_func(test_rollback,     &poly_neuron, "poly neuron rollback")   ||
       test_func(test_save_restore, &poly_neuron, "poly neuron save-restore")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}