#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "utils.h"

typedef struct {
    double inputs[4];
    double output;
} dataset_entry_t;

dataset_entry_t dataset[] = {
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

double get_error(neuron_params_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    double error = 0;
    neuron_reset_output_counter(config);
    for(size_t i=0; i<dataset_size; i++) {
        if(to_print) {
            printf("Stage %lld:\n", i);
        }
        double output = neuron_get_output(config, dataset[i].inputs, 0);

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
    double init_error = get_error(n, dataset, sizeof_arr(dataset), 0);
    // printf("Error before mutation = %f;\n", init_error);

    neuron_mutate(n);
    double error = get_error(n, dataset, sizeof_arr(dataset), 0);
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
    double init_error = get_error(n, dataset, sizeof_arr(dataset), 0);
    // printf("Error before mutation = %f;\n", init_error);
    neuron_stash_state(n);

    double error;
    for(uint32_t i=0; i<4; i++) {
        neuron_set_coeff(n, i, 0.5);
        error = get_error(n, dataset, sizeof_arr(dataset), 0);
        // printf("Error after setting coeff %d = %f;\n", i, error);
        neuron_rollback(n);
    }
    error = get_error(n, dataset, sizeof_arr(dataset), 0);
    // printf("Error after rollback = %f;\n", error);
    if(error != init_error) {
        printf("Error: rollback doesn't work, error != init_error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_save_restore(neuron_params_t *n) {
    double init_error = get_error(n, dataset, sizeof_arr(dataset), 0);
    neuron_save_coeffs(n, 123);
    neuron_set_coeff(n, 0, 0.5);
    neuron_set_coeff(n, 1, 0.5);
    neuron_set_coeff(n, 2, 0.5);
    neuron_set_coeff(n, 3, 0.5);
    // double init_error = get_error(n, dataset, sizeof_arr(dataset), 0);
    // printf("Error after setting coeffs = %f;\n", error);
    neuron_restore_data(n, 123);
    double error = get_error(n, dataset, sizeof_arr(dataset), 0);
    if(init_error != error) {
        printf("Error: save-restore doesn't work: error after restore != error before save");
        return EXIT_FAILURE;
    }
    // printf("Error after restore = %f;\n", error);
    return EXIT_SUCCESS;
}

void test_func(int(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    if(foo(n) == EXIT_FAILURE) {
        printf(" ERROR!\n");
        return;
    }
    printf("SUCCESS!\n");
}

int main() {
    srand(time(NULL));
    neuron_params_t test_neuron = {0};
    neuron_init(&test_neuron, 4, sizeof_arr(dataset));
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&test_neuron, j, j);
    }
    test_func(test_mutation,     &test_neuron, "mutations");
    test_func(test_rollback,     &test_neuron, "rollback");
    test_func(test_save_restore, &test_neuron, "save-restore");

    return 0;
}