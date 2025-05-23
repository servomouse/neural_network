#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "tests/neuron/common_functions.h"
#include "utils.h"
#include "tests/datasets.c"

#define DATASET polynome_dataset
#define DATASET_SIZE sizeof_arr(DATASET)

int test_adaptation(neuron_params_t *n) {
    double init_error = get_error(n, DATASET, DATASET_SIZE, 0);

    neuron_mutate(n);
    double error = get_error(n, DATASET, DATASET_SIZE, 0);
    if(error == init_error) {
        printf("Error: mutation doesn't work, error == init_error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    test_adaptation(&poly_neuron);
    return EXIT_SUCCESS;
}