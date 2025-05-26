#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

#define DATASET xor_dataset
#define DATASET_SIZE sizeof_arr(DATASET)

double test_coeffs[16][2] = {
    {0.0, 1.0},
    {1.0, 1.0},
    {1.0, 1.0},
    {0.0, 1.0},
    {1.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {1.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {0.0, 1.0},
    {-1.0, 0.25},
};

int test_with_coeffs(neuron_params_t *n) {
    neuron_set_coeffs(n, (void*)test_coeffs);
    double error = get_error(n, DATASET, DATASET_SIZE, 1);
    printf("Test with coeffs error: %f\n", error);
    return EXIT_SUCCESS;
}

int test_adaptation(neuron_params_t *n) {
    double error = get_error(n, DATASET, DATASET_SIZE, 1);
    uint32_t counter = 0, useful_mutations = 0;
    printf("Init error = %f\n", error);
    while((counter++ < 1000000) && (error > 0.01)) {
        neuron_mutate(n);
        double new_error = get_error(n, DATASET, DATASET_SIZE, 0);
        if(new_error > error) {
            neuron_rollback(n);
        } else {
            if(new_error < error) {
                useful_mutations++;
                if(useful_mutations == 1000) {
                    printf("Useful mutation! Error = %f\n", error);
                    useful_mutations = 0;
                }
            }
            error = new_error;
        }
    }
    printf("Final error = %f, counter = %d\n", get_error(n, DATASET, DATASET_SIZE, 1), counter);
    return EXIT_SUCCESS;
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    // test_with_coeffs(&poly_neuron);
    test_adaptation(&poly_neuron);
    neuron_print_coeffs(&poly_neuron);
    return EXIT_SUCCESS;
}