#include "common_functions.h"
#include "datasets.c"

int test_linear_output(neuron_params_t *n) {
    for(uint32_t i=0; i<16; i++) {
        double value = 0.02 * i+1;
        neuron_set_coeff(n, i, (void*)&value);
    }
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    if(round_to_precision(error, 6) == 0.517241) {
        return EXIT_SUCCESS;
    }
    printf("Error: error value differs from expected! Value = %f", error);
    return EXIT_FAILURE;
}

int test_poly_output(neuron_params_t *n) {
    for(uint32_t i=0; i<16; i++) {
        double vals[] = {0.02 * i+1, 1.0};
        neuron_set_coeff(n, i, (void*)&vals);
    }
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    if(round_to_precision(error, 6) == 0.517241) {
        return EXIT_SUCCESS;
    }
    printf("Error: error value differs from expected! Value = %f", error);
    return EXIT_FAILURE;
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func(test_poly_output, &poly_neuron, "poly neuron output")) {
        return EXIT_FAILURE;
    }
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_linear_output, &linear_neuron, "linear neuron output")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}