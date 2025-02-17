#include "common_functions.h"
#include "datasets.c"

int test_save_restore(neuron_params_t *n) {
    double init_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    // uint32_t num_coeffs = neuron_get_num_coeffs(n);
    // double *bckp_coeffs = calloc(num_coeffs, sizeof(double));
    // for(uint32_t i=0; i<num_coeffs; i++) {
    //     bckp_coeffs[i] = neuron_get_coeff(n, i);
    // }
    neuron_save(n, "neuron_backup.bin");
    neuron_set_coeff(n, 0, 0.5);
    neuron_set_coeff(n, 1, 0.5);
    neuron_set_coeff(n, 2, 0.5);
    neuron_set_coeff(n, 3, 0.5);
    double new_error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    if(init_error == new_error) {
        printf("Error: save-restore doesn't work: error after modification == init error!\n");
        return EXIT_FAILURE;
    }

    // printf("Error after setting coeffs = %f;\n", error);
    neuron_restore(n, "neuron_backup.bin");
    // neuron_set_coeffs(n, bckp_coeffs);
    double error = get_error(n, polynome_dataset, sizeof_arr(polynome_dataset), 0);
    if(init_error != error) {
        printf("Error: save-restore doesn't work: error after restore != error before modification");
        return EXIT_FAILURE;
    }
    // printf("Error after restore = %f;\n", error);
    return EXIT_SUCCESS;
}

int main() {
    srand(time(NULL));
    neuron_params_t poly_neuron = {0};
    neuron_init(&poly_neuron, NPoly, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&poly_neuron, j, j);
    }
    if(test_func(test_save_restore, &poly_neuron, "poly neuron save-restore")) {
        return EXIT_FAILURE;
    }
    neuron_params_t linear_neuron = {0};
    neuron_init(&linear_neuron, NLinear, 4);
    for(size_t j=0; j<4; j++) {
        neuron_set_input_idx(&linear_neuron, j, j);
    }
    if(test_func(test_save_restore, &linear_neuron, "linear neuron save-restore")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}