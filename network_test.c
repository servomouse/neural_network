#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network_new.h"

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

typedef struct {
    double inputs[4];
    double output[1];
} dataset_entry_t;

// dataset_entry_t dataset[] = {
//     {.inputs = {0.01, 0.05, 0.02, 0.03}, .output = {0.0}},
//     {.inputs = {0.07, 0.02, 0.05, 0.01}, .output = {0.0}},
//     {.inputs = {0.01, 0.05, 0.06, 0.04}, .output = {0.0}},
//     {.inputs = {0.02, 0.04, 0.07, 0.05}, .output = {0.0}},
//     {.inputs = {0.03, 0.03, 0.08, 0.06}, .output = {0.0}},
//     {.inputs = {0.04, 0.02, 0.09, 0.07}, .output = {0.0}},
//     {.inputs = {0.05, 0.01, 0.08, 0.98}, .output = {1.0}},
//     {.inputs = {0.06, 0.08, 0.97, 0.07}, .output = {1.0}},
//     {.inputs = {0.07, 0.07, 0.96, 0.96}, .output = {1.0}},
//     {.inputs = {0.08, 0.91, 0.05, 0.05}, .output = {1.0}},
//     {.inputs = {0.09, 0.92, 0.04, 0.95}, .output = {1.0}},
//     {.inputs = {0.01, 0.93, 0.93, 0.04}, .output = {1.0}},
//     {.inputs = {0.02, 0.94, 0.92, 0.93}, .output = {1.0}},
//     {.inputs = {0.91, 0.06, 0.01, 0.02}, .output = {1.0}},
//     {.inputs = {0.92, 0.05, 0.02, 0.91}, .output = {1.0}},
//     {.inputs = {0.93, 0.04, 0.93, 0.02}, .output = {1.0}},
//     {.inputs = {0.94, 0.03, 0.94, 0.93}, .output = {1.0}},
//     {.inputs = {0.95, 0.94, 0.05, 0.04}, .output = {1.0}},
//     {.inputs = {0.96, 0.95, 0.06, 0.95}, .output = {1.0}},
//     {.inputs = {0.97, 0.96, 0.97, 0.06}, .output = {1.0}},
//     {.inputs = {0.98, 0.97, 0.98, 0.97}, .output = {0.0}},
//     {.inputs = {0.97, 0.98, 0.99, 0.98}, .output = {0.0}},
//     {.inputs = {0.96, 0.97, 0.98, 0.97}, .output = {0.0}},
//     {.inputs = {0.95, 0.96, 0.97, 0.96}, .output = {0.0}},
//     {.inputs = {0.94, 0.95, 0.96, 0.95}, .output = {0.0}},
//     {.inputs = {0.93, 0.94, 0.95, 0.94}, .output = {0.0}},
//     {.inputs = {0.92, 0.93, 0.94, 0.93}, .output = {0.0}},
//     {.inputs = {0.91, 0.92, 0.93, 0.92}, .output = {0.0}},
//     {.inputs = {0.90, 0.91, 0.92, 0.91}, .output = {0.0}},
// };

// dataset_entry_t dataset[] = {
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = {0.0}},
//     {.inputs = {0.0, 0.0, 0.0, 1.0}, .output = {1.0}},
//     {.inputs = {0.0, 0.0, 1.0, 0.0}, .output = {1.0}},
//     {.inputs = {0.0, 0.0, 1.0, 1.0}, .output = {1.0}},
//     {.inputs = {0.0, 1.0, 0.0, 0.0}, .output = {1.0}},
//     {.inputs = {0.0, 1.0, 0.0, 1.0}, .output = {1.0}},
//     {.inputs = {0.0, 1.0, 1.0, 0.0}, .output = {1.0}},
//     {.inputs = {0.0, 1.0, 1.0, 1.0}, .output = {1.0}},
//     {.inputs = {1.0, 0.0, 0.0, 0.0}, .output = {1.0}},
//     {.inputs = {1.0, 0.0, 0.0, 1.0}, .output = {1.0}},
//     {.inputs = {1.0, 0.0, 1.0, 0.0}, .output = {1.0}},
//     {.inputs = {1.0, 0.0, 1.0, 1.0}, .output = {1.0}},
//     {.inputs = {1.0, 1.0, 0.0, 0.0}, .output = {1.0}},
//     {.inputs = {1.0, 1.0, 0.0, 1.0}, .output = {1.0}},
//     {.inputs = {1.0, 1.0, 1.0, 0.0}, .output = {1.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
//     {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = {0.0}},
// };



dataset_entry_t dataset[] = {
    {.inputs = {-1, 1.0, -1, 1.0}, .output = {1.0}},
    {.inputs = {-1, 1.0, 0.9, 0.8}, .output = {-0.3732}},
    {.inputs = {-0.9, -0.6, 0.9, 1.0}, .output = {0.1148}},
    {.inputs = {0.9, 0.8, -1, 0.7}, .output = {-0.128}},
    {.inputs = {0.9, -0.8, 1.0, -0.9}, .output = {0.2721}},
    {.inputs = {-1, -0.8, -0.7, 1.0}, .output = {-0.1756}},
    {.inputs = {0.7, -0.8, 1.0, 1.0}, .output = {-0.1756}},
    {.inputs = {0.9, -0.8, 1.0, 0.9}, .output = {-0.2721}},
    {.inputs = {-1, -0.8, 1.0, -0.7}, .output = {-0.1756}},
    {.inputs = {-1, -1, 0.9, 1.0}, .output = {0.729}},
    {.inputs = {-0.8, -0.9, -1, -0.9}, .output = {0.2721}},
    {.inputs = {-0.9, 0.8, 1.0, 0.8}, .output = {-0.1911}},
    {.inputs = {-0.9, 0.9, -0.8, -1}, .output = {-0.2721}},
    {.inputs = {1.0, 1.0, 0.9, -0.9}, .output = {-0.5314}},
    {.inputs = {1.0, 1.0, -0.6, 0.8}, .output = {-0.1106}},
    {.inputs = {0.8, 0.9, -1, 0.7}, .output = {-0.128}},
    {.inputs = {1.0, 0.8, -0.8, -0.9}, .output = {0.1911}},
    {.inputs = {-0.8, 0.9, 1.0, -0.9}, .output = {0.2721}},
    {.inputs = {-1, -1, -1, -0.6}, .output = {0.216}},
    {.inputs = {-0.6, -1, -1, 1.0}, .output = {-0.216}},
    {.inputs = {1.0, 0.6, -0.9, -0.9}, .output = {0.1148}},
    {.inputs = {-1, 0.8, -0.9, 0.7}, .output = {0.128}},
    {.inputs = {0.8, 0.8, 1.0, 0.9}, .output = {0.1911}},
    {.inputs = {1.0, 1.0, 1.0, 0.5}, .output = {0.125}},
    {.inputs = {0.8, -0.7, -1, 0.9}, .output = {0.128}},
    {.inputs = {0.9, -1, -0.8, 0.9}, .output = {0.2721}},
    {.inputs = {-0.7, 0.9, 1.0, 0.8}, .output = {-0.128}},
    {.inputs = {-0.7, -0.8, 1.0, 1.0}, .output = {0.1756}},
    {.inputs = {-1, -1, -0.8, -0.9}, .output = {0.3732}},
    {.inputs = {-1, -1, -0.9, 0.8}, .output = {-0.3732}},
    {.inputs = {1.0, -1, -0.8, 0.7}, .output = {0.1756}},
    {.inputs = {-0.9, -0.6, -1, 0.9}, .output = {-0.1148}},
    {.inputs = {-1, 1.0, 0.9, -0.7}, .output = {0.25}},
    {.inputs = {-1, -1, 0.6, -0.9}, .output = {-0.1575}},
    {.inputs = {1.0, -1, 0.9, -1}, .output = {0.729}},
    {.inputs = {-0.9, 0.7, -1, -0.9}, .output = {-0.1823}},
    {.inputs = {-0.8, -1, 0.8, 0.8}, .output = {0.1342}},
    {.inputs = {0.7, -1, 1.0, 1.0}, .output = {-0.343}},
    {.inputs = {0.9, -1, 1.0, 1.0}, .output = {-0.729}},
    {.inputs = {-0.9, 0.6, -1, 1.0}, .output = {0.1575}},
    {.inputs = {-0.9, 0.8, -0.9, -1}, .output = {-0.2721}},
    {.inputs = {-0.6, 0.8, -1, -1}, .output = {-0.1106}},
    {.inputs = {-0.9, 0.8, 0.9, 0.8}, .output = {-0.1393}},
    {.inputs = {-0.9, -0.8, 1.0, 0.9}, .output = {0.2721}},
    {.inputs = {1.0, 0.7, 0.7, 1.0}, .output = {0.1176}},
    {.inputs = {0.9, 0.7, -0.9, 0.9}, .output = {-0.1329}},
    {.inputs = {-0.9, 1.0, 0.9, 1.0}, .output = {-0.5314}},
    {.inputs = {0.9, -0.9, -0.6, 1.0}, .output = {0.1148}},
    {.inputs = {0.6, 0.9, 1.0, -1}, .output = {-0.1575}},
    {.inputs = {0.7, 1.0, -1, 0.8}, .output = {-0.1756}},
    {.inputs = {0.7, 0.7, 1.0, -1}, .output = {-0.1176}},
    {.inputs = {-1, 0.9, -1, -0.7}, .output = {-0.25}},
    {.inputs = {0.8, 1.0, 0.9, -1}, .output = {-0.3732}},
    {.inputs = {0.7, 1.0, 0.8, -0.9}, .output = {-0.128}},
    {.inputs = {0.8, -1, 0.9, -1}, .output = {0.3732}},
    {.inputs = {1.0, -1, 0.7, 0.8}, .output = {-0.1756}},
};

network_map_t network_map = {
    .num_inputs = 4,
    .net_size = 9,
    .num_outputs = 1,
    .neurons = {
        4, 4, NOT_OUTPUT, 0, 1, 2, 3,
        5, 4, NOT_OUTPUT, 0, 1, 2, 3,
        6, 4, NOT_OUTPUT, 0, 1, 2, 3,
        7, 4, NOT_OUTPUT, 0, 1, 2, 3,
        8, 4, 0,          4, 5, 6, 7,
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
        // if(to_print) {
        //     printf("Stage %lld:\n", i);
        // }
        double *outputs = network_get_outputs(config, dataset[i].inputs, 0);
        double e = 0;

        for(uint32_t j=0; j<num_outputs; j++) {
            double diff = dataset[i].output[j] - outputs[j];
            e += diff * diff;
        }
        e /= num_outputs;
        network_set_global_error(config, e);

        if(to_print) {
            // printf("Global error: %f\n\n", e);
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
    double init_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    double current_error = init_error;
    printf("Init error: %f\n", current_error);
    network_backup(&config);

    // size_t counter = 0;
    // while((current_error > 0.001) && (counter++ < 10000)) {
    //     // network_set_global_error(&config, current_error);
    //     network_mutate_micronet(&config);
    //     network_update(&config);
    //     // network_reset_counters(&config);
    //     double new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
    //     // printf("Error after mutation: %f\n", current_error);
    //     if(new_error > current_error) {
    //         network_restore(&config);
    //         network_rollback_micronet(&config);
    //     } else if((0.000001 + new_error) < current_error) {
    //         current_error = new_error;
    //         // error_delta = init_error - current_error;
    //         printf("Current error: %f\n", current_error);
    //         network_backup(&config);
    //     }
    // }
    // current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    // printf("Error after evolution: %f\n", current_error);

    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 100000)) {
        network_mutate(&config);
        double new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
        // printf("New error: %f\n", new_error);
        if(new_error > current_error) {
            network_rollback(&config);
        } else {
            if(new_error < current_error) {
                printf("New error: %f\n", new_error);
            }
            // printf("New error: %f\n", new_error);
            // if((counter % 1000) == 0) {
            //     printf("New error: %f\n", new_error);
            // }
            current_error = new_error;
        }
    }
    
    // network_print_coeffs(&config);
    get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 1);
    printf("Final error: %f, counter = %lld\n", current_error, counter);
    network_save_to_file(&config, "lalala");
    return 0;
}