#include "common_functions.h"
#include "datasets.c"

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

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func(test_mutation, &poly_neuron, "poly neuron mutations")) {
        return EXIT_FAILURE;
    }
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_mutation, &linear_neuron, "linear neuron mutations")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}