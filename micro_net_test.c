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
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.0},
    {.inputs = {0.0, 0.0, 0.0, 1.0}, .output = 1.0},
    {.inputs = {0.0, 0.0, 1.0, 0.0}, .output = 1.0},
    {.inputs = {0.0, 0.0, 1.0, 1.0}, .output = 1.0},
    {.inputs = {0.0, 1.0, 0.0, 0.0}, .output = 1.0},
    {.inputs = {0.0, 1.0, 0.0, 1.0}, .output = 1.0},
    {.inputs = {0.0, 1.0, 1.0, 0.0}, .output = 1.0},
    {.inputs = {0.0, 1.0, 1.0, 1.0}, .output = 1.0},
    {.inputs = {1.0, 0.0, 0.0, 0.0}, .output = 1.0},
    {.inputs = {1.0, 0.0, 0.0, 1.0}, .output = 1.0},
    {.inputs = {1.0, 0.0, 1.0, 0.0}, .output = 1.0},
    {.inputs = {1.0, 0.0, 1.0, 1.0}, .output = 1.0},
    {.inputs = {1.0, 1.0, 0.0, 0.0}, .output = 1.0},
    {.inputs = {1.0, 1.0, 0.0, 1.0}, .output = 1.0},
    {.inputs = {1.0, 1.0, 1.0, 0.0}, .output = 1.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.0},
};

micronet_map_t micronet_map = {
    .num_inputs = 4,
    .num_neurons = 1,
    .net_size = 5,
    .neurons = {
        {.idx = 4, .num_inputs = 4, .indices = {0, 1, 2, 3, 0, 0, 0, 0}},
        {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
    .output_idx = 4
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

int main(void) {
    srand(time(NULL));
    micro_network_t config;
    micronet_init(&config, &micronet_map);
    printf("MicroNet initialised!\n");
    double current_error = get_error(&config, dataset, sizeof_arr(dataset), 1);
    printf("Init error: %f\n", current_error);
    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 10000)) {
        micronet_mutate(&config);
        double new_error = get_error(&config, dataset, sizeof_arr(dataset), 0);
        // printf("New error: %f\n", new_error);
        if(new_error > current_error) {
            micronet_rollback(&config);
        } else {
            // if(new_error < current_error) {
            //     printf("New error: %f\n", new_error);
            // }
            if((counter % 1000) == 0) {
                printf("New error: %f\n", new_error);
            }
            current_error = new_error;
        }
    }
    
    micronet_print_coeffs(&config);
    get_error(&config, dataset, sizeof_arr(dataset), 1);
    printf("Final error: %f, counter = %lld\n", current_error, counter);
    return 0;
}