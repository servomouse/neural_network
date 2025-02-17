#include "common_functions.h"
#include "datasets.c"

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

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func(test_rollback, &poly_neuron, "poly neuron rollback")) {
        return EXIT_FAILURE;
    }
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_rollback, &linear_neuron, "linear neuron rollback")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}