#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

#define DATASET xor_dataset
#define DATASET_SIZE sizeof_arr(DATASET)

int test_adaptation(neuron_params_t *n) {
    double error = get_error(n, DATASET, DATASET_SIZE, 1);
    uint32_t counter = 0;
    printf("Init error = %f\n", error);
    while((counter++ < 100000) && (error > 0.01)) {
        neuron_mutate(n);
        double new_error = get_error(n, DATASET, DATASET_SIZE, 0);
        if(new_error > error) {
            neuron_rollback(n);
        } else {
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
    test_adaptation(&poly_neuron);
    neuron_print_coeffs(&poly_neuron);
    return EXIT_SUCCESS;
}