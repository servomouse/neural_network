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
        {.idx = 4, .num_inputs = 4, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 5, .num_inputs = 6, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 6, .num_inputs = 6, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 8, .num_inputs = 6, .indices = {4, 5, 6, 7, 0, 0, 0, 0}},
    },
    .output_idx = 8
};

double get_error(micro_network_t *config, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
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

void test_micronet(micro_network_t *config) {
    double init_error = get_error(config, dataset, sizeof_arr(dataset), 0);
    printf("Init error: %f\n", init_error);
    micronet_mutate(config);
    double error_before = get_error(config, dataset, sizeof_arr(dataset), 0);
    if(error_before == init_error) {
        printf("Error: Mutation doesn't work: error after mutation == error before mutation!!! init_error = %f, error_before = %f", init_error, error_before);
        return;
    }
    micronet_rollback(config);
    double error_after = get_error(config, dataset, sizeof_arr(dataset), 0);
    if(error_before == error_after) {
        printf("Error: Rollback doesn't work: error after rollback == error before rollback!!! init_error = %f, error_before = %f, error_after = %f", init_error, error_before, error_after);
        return;
    } else if(error_after != init_error) {
        printf("Error: Rollback doesn't work: error after rollback != init_error!!! init_error = %f, error_before = %f, error_after = %f", init_error, error_before, error_after);
        return;
    }
    printf("Mutations work, test passed!");
}

int main(void) {
    srand(time(NULL));
    micro_network_t config;
    micronet_init(&config, &micronet_map);
    printf("MicroNet initialised!\n");
    test_micronet(&config);
    // double current_error = get_error(&config, dataset, sizeof_arr(dataset), 1);
    // printf("Init error: %f\n", current_error);
    // size_t counter = 0;
    // while((current_error > 0.001) && (counter++ < 10000)) {
    //     micronet_mutate(&config);
    //     double new_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    //     // printf("New error: %f\n", new_error);
    //     if(new_error > current_error) {
    //         micronet_rollback(&config);
    //     } else {
    //         if(new_error < current_error) {
    //             printf("New error: %f\n", new_error);
    //         }
    //         if((counter % 1000) == 0) {
    //             printf("New error: %f\n", new_error);
    //         }
    //         current_error = new_error;
    //     }
    // }
    
    // // micronet_print_coeffs(&config);
    // current_error = get_error(&config, dataset, sizeof_arr(dataset), 1);
    // printf("Final error: %f, counter = %lld\n", current_error, counter);
    return 0;
}