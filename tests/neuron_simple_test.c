#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "utils.h"
#include "dataset.h"

double get_error(neuron_params_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    double error = 0;
    // neuron_reset_output_counter(config);
    for(size_t i=0; i<dataset_size; i++) {
        if(to_print) {
            printf("Stage %lld:\n", i);
        }
        double output = neuron_get_output(config, dataset[i].inputs);

        double diff = dataset[i].output - output;
        double e = diff * diff;
        // network_set_global_error(config, e);

        if(to_print) {
            printf("Error: %f\n", e);
            printf("Desired output: %f; real output: %f;\n", dataset[i].output, output);
        }
        error += e;
    }
    return error / dataset_size;
}

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
    // error = get_error(n, dataset, sizeof_arr(dataset), 0);
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
    uint32_t num_coeffs = neuron_get_num_coeffs(n);
    double *bckp_coeffs = calloc(num_coeffs, sizeof(double));
    for(uint32_t i=0; i<num_coeffs; i++) {
        bckp_coeffs[i] = neuron_get_coeff(n, i);
    }
    // neuron_save_coeffs(n, 123);
    neuron_set_coeff(n, 0, 0.5);
    neuron_set_coeff(n, 1, 0.5);
    neuron_set_coeff(n, 2, 0.5);
    neuron_set_coeff(n, 3, 0.5);
    // double init_error = get_error(n, dataset, sizeof_arr(dataset), 0);
    // printf("Error after setting coeffs = %f;\n", error);
    // neuron_restore_data(n, 123);
    neuron_set_coeffs(n, bckp_coeffs);
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

int test_func(int(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    if(foo(n) == EXIT_FAILURE) {
        printf(" ERROR!\n");
        return EXIT_FAILURE;
    }
    printf("SUCCESS!\n");
    return EXIT_SUCCESS;
}

int main() {
    srand(time(NULL));
    neuron_params_t test_neuron = {0};
    neuron_init(&test_neuron, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&test_neuron, j, j);
    }
    if(test_func(test_output,       &test_neuron, "output") ||
       test_func(test_mutation,     &test_neuron, "mutations")||
       test_func(test_rollback,     &test_neuron, "rollback")||
       test_func(test_save_restore, &test_neuron, "save-restore")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}