#include "micronet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "datasets.c"

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

micronet_map_t micronet_map = {
    .num_inputs = 4,
    .num_neurons = 5,
    .net_size = 9,
    .neurons = neurons,
    .num_outputs = 1,
    .output_indices = {8},
};

static double get_error(micro_network_t *config, u_dataset_entry_t *dataset, size_t dataset_size, uint32_t num_outputs, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = micronet_get_output(config, dataset[i].inputs);
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

int test_basic_functions(micro_network_t *config) {
    double init_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    micronet_mutate(config);
    double error_before = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(error_before == init_error) {
        return EXIT_FAILURE;
    }
    micronet_rollback(config);
    double error_after = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(error_before == error_after) {
        return EXIT_FAILURE;
    } else if(error_after != init_error) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_multiple_mutations(micro_network_t *config) {
    double init_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    double error_before, error_after;
    for(uint32_t i=0; i<10000; i++) {
        micronet_mutate(config);
        error_before = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
        if(error_before != init_error) {
            micronet_rollback(config);
            error_after = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(error_before == error_after) {
                return EXIT_FAILURE;
            } else if(error_after != init_error) {
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

int test_evolution(micro_network_t *config) {
    double current_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    // double init_error = current_error;
    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 10000)) {
        micronet_mutate(config);
        double new_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
        if(new_error > current_error) {
            micronet_rollback(config);
            double temp_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(temp_error != current_error) {
                return EXIT_FAILURE;
            }
        } else {
            current_error = new_error;
            double temp_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
            if(temp_error != current_error) {
                return EXIT_FAILURE;
            }
        }
        if(new_error < current_error) {
            return EXIT_FAILURE;
        }
    }
    
    double after_error = get_error(config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
    if(after_error != current_error) {
        printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_func(int(*foo)(micro_network_t*), micro_network_t *n, const char *test_name) {
    printf("Testing %s . . . ", test_name);
    if(foo(n) == EXIT_FAILURE) {
        printf(" ERROR!\n");
        return EXIT_FAILURE;
    }
    printf("SUCCESS!\n");
    return EXIT_SUCCESS;
}

// int evolution(void) {
//     srand(time(NULL));
//     micro_network_t config;
//     micronet_init(&config, &micronet_map);
//     printf("MicroNet initialised!\n");

//     double current_error = get_error(&config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
//     // printf("Init error: %f\n", current_error);
//     size_t counter = 0;
//     while((current_error > 0.001) && (counter++ < 1000)) {
//         micronet_mutate(&config);
//         double new_error = get_error(&config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
//         // printf("New error: %f\n", new_error);
//         if(new_error > current_error) {
//             micronet_rollback(&config);
//             double temp_error = get_error(&config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
//             if(temp_error != current_error) {
//                 printf("ERROR1: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
//                 return 1;
//             }
//         } else {
//             // if(new_error < current_error) {
//             //     printf("New error: %f\n", new_error);
//             // }
//             // if((counter % 1000) == 0) {
//             //     printf("New error: %f\n", new_error);
//             // }
//             current_error = new_error;
//             double temp_error = get_error(&config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 0);
//             if(temp_error != current_error) {
//                 printf("ERROR2: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
//                 return 1;
//             }
//         }
//         if(new_error < current_error) {
//             printf("Error: new_error < current_error!!!! new_error = %f, current_error = %f\n", new_error, current_error);
//             return 1;
//         }
//         // if(counter++ < 10000) {
//         //     break;
//         // }
//     }
    
//     // micronet_print_coeffs(&config);
//     double after_error = get_error(&config, micronet_dataset, sizeof_arr(micronet_dataset), 1, 1);
//     if(after_error != current_error) {
//         printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
//         return 1;
//     }
//     printf("Final error: %f, counter = %lld\n", after_error, counter);
//     return 0;
// }

int main(void) {
    srand(time(NULL));
    micro_network_t config = {0};
    micronet_init(&config, &micronet_map);
    printf("MicroNet initialised!\n");

    if(test_func(test_basic_functions,      &config, "basic functions") ||
       test_func(test_multiple_mutations,   &config, "multiple mutations")||
       test_func(test_evolution,            &config, "evolution")) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}