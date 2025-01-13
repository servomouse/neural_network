#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network_new.h"

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

typedef struct {
    double inputs[4];
    double output[1];
} dataset_entry_t;

dataset_entry_t dataset[] = {
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
    {.inputs = {0.0, 0.0, 0.0, 1.0}, .output = {1.0}},
    {.inputs = {0.0, 0.0, 1.0, 0.0}, .output = {1.0}},
    {.inputs = {0.0, 0.0, 1.0, 1.0}, .output = {1.0}},
    {.inputs = {0.0, 1.0, 0.0, 0.0}, .output = {1.0}},
    {.inputs = {0.0, 1.0, 0.0, 1.0}, .output = {1.0}},
    {.inputs = {0.0, 1.0, 1.0, 0.0}, .output = {1.0}},
    {.inputs = {0.0, 1.0, 1.0, 1.0}, .output = {1.0}},
    {.inputs = {1.0, 0.0, 0.0, 0.0}, .output = {1.0}},
    {.inputs = {1.0, 0.0, 0.0, 1.0}, .output = {1.0}},
    {.inputs = {1.0, 0.0, 1.0, 0.0}, .output = {1.0}},
    {.inputs = {1.0, 0.0, 1.0, 1.0}, .output = {1.0}},
    {.inputs = {1.0, 1.0, 0.0, 0.0}, .output = {1.0}},
    {.inputs = {1.0, 1.0, 0.0, 1.0}, .output = {1.0}},
    {.inputs = {1.0, 1.0, 1.0, 0.0}, .output = {1.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
};

network_map_t network_map = {
    .num_inputs = 4,
    .net_size = 5,
    .num_outputs = 1,
    .neurons = {
        4, 4, 0, 0, 1, 2, 3,
        // {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
};

double get_error(network_t *config, uint32_t num_outputs, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    network_reset_counters(config);
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = network_get_outputs(config, dataset[i].inputs);
        double e = 0;

        for(uint32_t j=0; j<num_outputs; j++) {
            double diff = dataset[i].output[j] - outputs[j];
            e += diff * diff;
        }
        e /= num_outputs;

        if(to_print) {
            printf("Desired outputs: [");
            for(uint32_t j=0; j<num_outputs; j++) {
                printf("%f, ", dataset[i].output[j]);
            }
            printf("]; real outputs: [");
            for(uint32_t j=0; j<num_outputs; j++) {
                printf("%f, ", outputs[j]);
            }
            printf("]\n");
        }
        error += e;
    }
    return error / dataset_size;
}

// get init results
// save network state
// mutate micronet
// train network
// get new results
// compare with init
// if results are worse: rollback the mutation, restore the network


int main(void) {
    srand(time(NULL));
    network_t config;
    network_init(&config, &network_map, sizeof_arr(dataset));
    printf("Network initialised!\n");
    double current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    printf("Init error: %f\n", current_error);
    network_backup(&config);
    network_mutate_micronet(&config);
    network_update(&config);
    current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    printf("Error after mutation: %f\n", current_error);
    network_restore(&config);
    current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    printf("Error after rollback: %f\n", current_error);
    // size_t counter = 0;
    // while((current_error > 0.001) && (counter++ < 10000)) {
    //     micronet_mutate(&config);
    //     double new_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
    //     // printf("New error: %f\n", new_error);
    //     if(new_error > current_error) {
    //         micronet_rollback(&config);
    //     } else {
    //         // if(new_error < current_error) {
    //         //     printf("New error: %f\n", new_error);
    //         // }
    //         if((counter % 1000) == 0) {
    //             printf("New error: %f\n", new_error);
    //         }
    //         current_error = new_error;
    //     }
    // }
    
    // micronet_print_coeffs(&config);
    // get_error(&config, dataset, sizeof_arr(dataset), 1);
    // printf("Final error: %f, counter = %lld\n", current_error, counter);
    return 0;
}