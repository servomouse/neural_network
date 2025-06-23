#include "common_functions.h"

int test_func(int(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    if(foo(n) == EXIT_FAILURE) {
        printf(" ERROR!\n");
        return EXIT_FAILURE;
    }
    printf("SUCCESS!\n");
    return EXIT_SUCCESS;
}

int test_func_no_ret(void(*foo)(neuron_params_t*), neuron_params_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    foo(n);
    printf("SUCCESS!\n");
    return EXIT_SUCCESS;
}

double get_error(neuron_params_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    double error = 0;
    // neuron_reset_output_counter(config);
    for(size_t i=0; i<dataset_size; i++) {
        // if(to_print) {
        //     printf("Stage %lld:\n", i);
        // }
        double output = neuron_get_output(config, dataset[i].inputs);

        double diff = dataset[i].output - output;
        double e = diff * diff;
        // network_set_global_error(config, e);

        if(to_print) {
            // printf("Error: %f\n", e);
            printf("Desired output: %f; real output: %f;\n", dataset[i].output, output);
        }
        error += e;
    }
    return round_to_precision(error / dataset_size, 6);
}
