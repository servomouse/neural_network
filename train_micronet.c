#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network.h"
#include "utils.h"

// Micronet config:
#include "dataset.h"
#include "micro_maps/config_td_network.h"

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

// Neuron types:
// 0 - linear
// 1 - polynomial

// uint32_t neurons[] = {
//     // idx  num_inputs  type indices
//        4,   4,          1,   0, 1, 2, 3,
//        5,   4,          1,   0, 1, 2, 3,
//        6,   4,          1,   0, 1, 2, 3,
//        7,   4,          1,   0, 1, 2, 3,
//        8,   4,          1,   4, 5, 6, 7,
// };

// network_map_t micronet_map = {
//     .num_inputs = 4,
//     .num_neurons = 5,
//     .net_size = 9,
//     .neurons = neurons,
//     .num_outputs = 1,
//     .output_indices = {8},
// };

static double get_error(network_t *config, dataset_item_t *dataset, size_t dataset_size, uint32_t num_outputs, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = network_get_output(config, dataset[i].inputs);
        double delta = dataset[i].output - outputs[0];
        double e = delta * delta;
        if(to_print) {
            printf("Desired outputs: [");
            printf("%f], ", dataset[i].output);

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

int run_evolution(network_t *config) {
    double current_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
    size_t counter = 0;
    while((current_error > 0.001) && (counter++ < 10000)) {
        network_mutate(config);
        double new_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
        if(new_error > current_error) {
            network_rollback(config);
        } else {
            current_error = new_error;
            printf("New error: %f percent, counter = %d\n", 100*new_error, counter);
            fflush(stdout);
        }
    }
    
    // double after_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
    // if(after_error != current_error) {
    //     printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}

int main(void) {
    srand(time(NULL));
    network_t *unet = calloc(1, sizeof(network_t));
    network_init_micronet(unet, &network_map);
    printf("MicroNet initialised!\n");
    run_evolution(unet);

    // Save network
    char *n_path = concat_strings(BCKP_DIR_PATH, "/td_micronet");
    network_save(unet, n_path);

    return EXIT_SUCCESS;
}