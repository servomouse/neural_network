#include "common_functions.h"
#include "datasets.c"

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
    if(test_func(test_output, &poly_neuron, "poly neuron output")) {
        return EXIT_FAILURE;
    }
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_output, &linear_neuron, "linear neuron output")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}