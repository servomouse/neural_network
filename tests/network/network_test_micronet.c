#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network_test_micronet.h"
#include "lib/utils.h"

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

// Neuron types:
// 0 - linear
// 1 - polynomial

uint32_t neurons[] = {
    // idx  num_inputs  type indices
       4,   4,          1,   0, 1, 2, 3,
       5,   4,          1,   0, 1, 2, 3,
       6,   4,          1,   0, 1, 2, 3,
       7,   4,          1,   0, 1, 2, 3,
       8,   4,          1,   4, 5, 6, 7,
};

network_map_t micronet_map = {
    .num_inputs = 4,
    .num_neurons = 5,
    .neurons = neurons,
    .num_outputs = 1,
    .output_indices = {8},
};

static double get_error(network_t *config, u_dataset_entry_t *dataset, size_t dataset_size, uint32_t num_outputs, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = network_get_output(config, dataset[i].inputs);
        double e = 0.0;
        for(uint32_t j=0; j<num_outputs; j++) {
            double delta = dataset[i].output[j] - outputs[j];
            e += delta * delta;
        }
        if(to_print) {
            printf("Desired outputs: [");
            for(uint32_t j=0; j<num_outputs-1; j++) {
                printf("%f, ", dataset[i].output[j]);
            }
            printf("%f], ", dataset[i].output[num_outputs-1]);

            printf("real outputs: [");
            for(uint32_t j=0; j<num_outputs-1; j++) {
                printf("%f, ", outputs[j]);
            }
            printf("%f];\n", outputs[num_outputs-1]);
        }
        error += e;
    }
    return error / dataset_size;
}

void test_basic_functions(network_t *config) {
    double init_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    network_mutate(config);
    double error_before = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(are_equal(error_before, init_error, 6)) {
        RAISE("Error: error after mutation == initial error! Init_error: %f, error_before: %f\n", init_error, error_before);
    }
    network_rollback(config);
    double error_after = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(error_before == error_after) {
        RAISE("Error: error after rollback == error after mutation!\n");
    } else if(error_after != init_error) {
        RAISE("Error: error after rollback != initial error!\n");
    }
}

void test_multiple_mutations(network_t *config) {
    double init_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    double error_before, error_after;
    for(uint32_t i=0; i<10000; i++) {
        network_mutate(config);
        error_before = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
        if(error_before != init_error) {
            network_rollback(config);
            error_after = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(error_before == error_after) {
                RAISE("Error: error_before == init_error (%f == %f)\n", error_before, init_error);
            } else if(error_after != init_error) {
                RAISE("Error: error_after != init_error (%f != %f)\n", error_after, init_error);
            }
        }
    }
}

void test_evolution(network_t *config) {
    double current_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 10000)) {
        network_mutate(config);
        double new_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
        if(new_error > current_error) {
            network_rollback(config);
            double temp_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(temp_error != current_error) {
                RAISE("Error: temp_error != current_error (%f != %f)\n", temp_error, current_error);
            }
        } else {
            current_error = new_error;
            double temp_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(temp_error != current_error) {
                RAISE("Error: temp_error != current_error (%f != %f)\n", temp_error, current_error);
            }
        }
        if(new_error < current_error) {
            RAISE("Error: new_error < current_error (%f < %f)\n", new_error, current_error);
        }
    }
    
    double after_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(after_error != current_error) {
        RAISE("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
    }
}

int test_func(void(*foo)(network_t*), network_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    foo(n);
    printf("SUCCESS!\n");
    return EXIT_SUCCESS;
}

int main(void) {
    srand(time(NULL));
    network_t *unet = calloc(1, sizeof(network_t));
    network_init_micronet(unet, &micronet_map);
    printf("MicroNet initialised!\n");

    if(
       test_func(test_evolution,             unet, "evolution")     // First make the network output something useful
    //    || test_func(test_basic_functions,    unet, "basic functions")
    //    || test_func(test_multiple_mutations, unet, "multiple mutations")
       ) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}