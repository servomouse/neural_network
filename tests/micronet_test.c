#include "micro_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

typedef struct {
    double inputs[4];
    double output;
} dataset_entry_t;

dataset_entry_t dataset[] = {
    {.inputs = {-1.0, 1.0, -1.0, 1.0},   .output = 1.0},
    {.inputs = {-1.0, 1.0, 0.9, 0.8},    .output = -0.3732},
    {.inputs = {-0.9, -0.6, 0.9, 1.0},   .output = 0.1148},
    {.inputs = {0.9, 0.8, -1.0, 0.7},    .output = -0.128},
    {.inputs = {0.9, -0.8, 1.0, -0.9},   .output = 0.2721},
    {.inputs = {-1.0, -0.8, -0.7, 1.0},  .output = -0.1756},
    {.inputs = {0.7, -0.8, 1.0, 1.0},    .output = -0.1756},
    {.inputs = {0.9, -0.8, 1.0, 0.9},    .output = -0.2721},
    {.inputs = {-1.0, -0.8, 1.0, -0.7},  .output = -0.1756},
    {.inputs = {-1.0, -1.0, 0.9, 1.0},   .output = 0.729},
    {.inputs = {-0.8, -0.9, -1.0, -0.9}, .output = 0.2721},
    {.inputs = {-0.9, 0.8, 1.0, 0.8},    .output = -0.1911},
    {.inputs = {-0.9, 0.9, -0.8, -1},    .output = -0.2721},
    {.inputs = {1.0, 1.0, 0.9, -0.9},    .output = -0.5314},
    {.inputs = {1.0, 1.0, -0.6, 0.8},    .output = -0.1106},
    {.inputs = {0.8, 0.9, -1.0, 0.7},    .output = -0.128},
    {.inputs = {1.0, 0.8, -0.8, -0.9},   .output = 0.1911},
    {.inputs = {-0.8, 0.9, 1.0, -0.9},   .output = 0.2721},
    {.inputs = {-1.0, -1.0, -1.0, -0.6}, .output = 0.216},
    {.inputs = {-0.6, -1.0, -1.0, 1.0},  .output = -0.216},
    {.inputs = {1.0, 0.6, -0.9, -0.9},   .output = 0.1148},
    {.inputs = {-1.0, 0.8, -0.9, 0.7},   .output = 0.128},
    {.inputs = {0.8, 0.8, 1.0, 0.9},     .output = 0.1911},
    {.inputs = {1.0, 1.0, 1.0, 0.5},     .output = 0.125},
    {.inputs = {0.8, -0.7, -1.0, 0.9},   .output = 0.128},
    {.inputs = {0.9, -1.0, -0.8, 0.9},   .output = 0.2721},
    {.inputs = {-0.7, 0.9, 1.0, 0.8},    .output = -0.128},
    {.inputs = {-0.7, -0.8, 1.0, 1.0},   .output = 0.1756},
    {.inputs = {-1.0, -1.0, -0.8, -0.9}, .output = 0.3732},
    {.inputs = {-1.0, -1.0, -0.9, 0.8},  .output = -0.3732},
    {.inputs = {1.0, -1.0, -0.8, 0.7},   .output = 0.1756},
    {.inputs = {-0.9, -0.6, -1.0, 0.9},  .output = -0.1148},
    {.inputs = {-1.0, 1.0, 0.9, -0.7},   .output = 0.25},
    {.inputs = {-1.0, -1.0, 0.6, -0.9},  .output = -0.1575},
    {.inputs = {1.0, -1.0, 0.9, -1},     .output = 0.729},
    {.inputs = {-0.9, 0.7, -1.0, -0.9},  .output = -0.1823},
    {.inputs = {-0.8, -1.0, 0.8, 0.8},   .output = 0.1342},
    {.inputs = {0.7, -1.0, 1.0, 1.0},    .output = -0.343},
    {.inputs = {0.9, -1.0, 1.0, 1.0},    .output = -0.729},
    {.inputs = {-0.9, 0.6, -1.0, 1.0},   .output = 0.1575},
    {.inputs = {-0.9, 0.8, -0.9, -1},    .output = -0.2721},
    {.inputs = {-0.6, 0.8, -1.0, -1},    .output = -0.1106},
    {.inputs = {-0.9, 0.8, 0.9, 0.8},    .output = -0.1393},
    {.inputs = {-0.9, -0.8, 1.0, 0.9},   .output = 0.2721},
    {.inputs = {1.0, 0.7, 0.7, 1.0},     .output = 0.1176},
    {.inputs = {0.9, 0.7, -0.9, 0.9},    .output = -0.1329},
    {.inputs = {-0.9, 1.0, 0.9, 1.0},    .output = -0.5314},
    {.inputs = {0.9, -0.9, -0.6, 1.0},   .output = 0.1148},
    {.inputs = {0.6, 0.9, 1.0, -1},      .output = -0.1575},
    {.inputs = {0.7, 1.0, -1.0, 0.8},    .output = -0.1756},
    {.inputs = {0.7, 0.7, 1.0, -1},      .output = -0.1176},
    {.inputs = {-1.0, 0.9, -1.0, -0.7},  .output = -0.25},
    {.inputs = {0.8, 1.0, 0.9, -1},      .output = -0.3732},
    {.inputs = {0.7, 1.0, 0.8, -0.9},    .output = -0.128},
    {.inputs = {0.8, -1.0, 0.9, -1},     .output = 0.3732},
    {.inputs = {1.0, -1.0, 0.7, 0.8},    .output = -0.1756},
};

micronet_map_t micronet_map = {
    .num_inputs = 4,
    .num_neurons = 5,
    .net_size = 9,
    .neurons = {
        {.idx = 4, .num_inputs = 4, .indices = {0, 1, 2, 3}},
        {.idx = 5, .num_inputs = 4, .indices = {0, 1, 2, 3}},
        {.idx = 6, .num_inputs = 4, .indices = {0, 1, 2, 3}},
        {.idx = 7, .num_inputs = 4, .indices = {0, 1, 2, 3}},
        {.idx = 8, .num_inputs = 4, .indices = {4, 5, 6, 7}},
    },
    .output_idx = 8
};

static double get_error(micro_network_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double output = micronet_get_output(config, dataset[i].inputs);
        double e = (dataset[i].output - output) * (dataset[i].output - output);
        if(to_print) {
            printf("Desired output: %f; real output: %f\n", dataset[i].output, output);
        }
        error += e;
    }
    return error / dataset_size;
}

int test_basic_functions(micro_network_t *config) {
    double init_error = get_error(config, dataset, sizeof_arr(dataset), 0);
    micronet_mutate(config);
    double error_before = get_error(config, dataset, sizeof_arr(dataset), 0);
    if(error_before == init_error) {
        // printf("Error: Mutation doesn't work: error after mutation == error before mutation!!! init_error = %f, error_before = %f\n", init_error, error_before);
        return EXIT_FAILURE;
    }
    micronet_rollback(config);
    double error_after = get_error(config, dataset, sizeof_arr(dataset), 0);
    if(error_before == error_after) {
        // printf("Error: Rollback doesn't work: error after rollback == error before rollback!!! init_error = %f, error_before = %f, error_after = %f\n", init_error, error_before, error_after);
        return EXIT_FAILURE;
    } else if(error_after != init_error) {
        // printf("Error: Rollback doesn't work: error after rollback != init_error!!! init_error = %f, error_before = %f, error_after = %f\n", init_error, error_before, error_after);
        return EXIT_FAILURE;
    }
    // printf("Mutations work, test passed!\n");
    return EXIT_SUCCESS;
}

int test_multiple_mutations(micro_network_t *config) {
    double init_error = get_error(config, dataset, sizeof_arr(dataset), 0);
    // printf("Init error: %f\n", init_error);
    double error_before, error_after;
    for(uint32_t i=0; i<10000; i++) {
        micronet_mutate(config);
        error_before = get_error(config, dataset, sizeof_arr(dataset), 0);
        if(error_before != init_error) {
            // printf("Error: Mutation doesn't work: error after mutation == error before mutation!!! init_error = %f, error_before = %f, i = %d\n",
            //         init_error, error_before, i);
            // return;
            micronet_rollback(config);
            error_after = get_error(config, dataset, sizeof_arr(dataset), 0);
            if(error_before == error_after) {
                // printf("Error: Rollback doesn't work: error after rollback == error before rollback!!! init_error = %f, error_before = %f, error_after = %f, i = %d\n",
                //         init_error, error_before, error_after, i);
                return EXIT_FAILURE;
            } else if(error_after != init_error) {
                // printf("Error: Rollback doesn't work: error after rollback != init_error!!! init_error = %f, error_before = %f, error_after = %f, i = %d\n",
                //         init_error, error_before, error_after, i);
                return EXIT_FAILURE;
            }
        }
    }
    // printf("Multiple mutations work, test passed!\n");
    return EXIT_SUCCESS;
}

int test_evolution(micro_network_t *config) {
    double current_error = get_error(config, dataset, sizeof_arr(dataset), 0);
    // double init_error = current_error;
    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 10000)) {
        micronet_mutate(config);
        double new_error = get_error(config, dataset, sizeof_arr(dataset), 0);
        if(new_error > current_error) {
            micronet_rollback(config);
            double temp_error = get_error(config, dataset, sizeof_arr(dataset), 0);
            if(temp_error != current_error) {
                // printf("ERROR1: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
                return EXIT_FAILURE;
            }
        } else {
            // if(new_error < current_error) {
            //     printf("New error: %f\n", new_error);
            // }
            current_error = new_error;
            double temp_error = get_error(config, dataset, sizeof_arr(dataset), 0);
            if(temp_error != current_error) {
                // printf("ERROR2: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
                return EXIT_FAILURE;
            }
        }
        if(new_error < current_error) {
            // printf("Error: new_error < current_error!!!! new_error = %f, current_error = %f\n", new_error, current_error);
            return EXIT_FAILURE;
        }
    }
    
    double after_error = get_error(config, dataset, sizeof_arr(dataset), 0);
    if(after_error != current_error) {
        printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
        return EXIT_FAILURE;
    }
    // printf("Init error: %f, final error: %f, counter = %lld\n", init_error, after_error, counter);
    // printf("Evolution works, test passed!\n");
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

int main(void) {
    srand(time(NULL));
    micro_network_t config;
    micronet_init(&config, &micronet_map, NULL);
    printf("MicroNet initialised!\n");

    // micronet_get_output(&config, dataset[0].inputs);

    if(
        test_func(test_basic_functions,      &config, "basic functions") ||
        test_func(test_multiple_mutations,   &config, "multiple mutations")||
        test_func(test_evolution,            &config, "evolution")
        ) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    // test_micronet(&config);
    // test_multiple_mutations(&config);
    // test_evolution(&config);

    // double current_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    // // printf("Init error: %f\n", current_error);
    // size_t counter = 0;
    // while((current_error > 0.001) && (counter++ < 10000)) {
    //     micronet_mutate(&config);
    //     double new_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    //     // printf("New error: %f\n", new_error);
    //     if(new_error > current_error) {
    //         micronet_rollback(&config);
    //         double temp_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    //         if(temp_error != current_error) {
    //             printf("ERROR1: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
    //             return 1;
    //         }
    //     } else {
    //         // if(new_error < current_error) {
    //         //     printf("New error: %f\n", new_error);
    //         // }
    //         // if((counter % 1000) == 0) {
    //         //     printf("New error: %f\n", new_error);
    //         // }
    //         current_error = new_error;
    //         double temp_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    //         if(temp_error != current_error) {
    //             printf("ERROR2: temp_error != current_error: temp_error = %f, current_error = %f\n", temp_error, current_error);
    //             return 1;
    //         }
    //     }
    //     if(new_error < current_error) {
    //         printf("Error: new_error < current_error!!!! new_error = %f, current_error = %f\n", new_error, current_error);
    //         return 1;
    //     }
    //     // if(counter++ < 10000) {
    //     //     break;
    //     // }
    // }
    
    // // micronet_print_coeffs(&config);
    // double after_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    // if(after_error != current_error) {
    //     printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
    //     return 1;
    // }
    // printf("Final error: %f, counter = %lld\n", after_error, counter);
    return 0;
}