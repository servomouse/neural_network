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

double update_value(double value) {
    double temp = value * 100;
    if(temp > 1) {temp = 1;}
    else if(temp < -1) {temp = -1;}
    return temp;
}

static double get_error(network_t *config, dataset_item_t *dataset, size_t dataset_size, uint32_t num_outputs, uint8_t to_print) {
    // Works only with single output networks
    double error = 0;
    uint32_t correct_sign_count = 0;
    uint32_t correct_high_count = 0;
    uint32_t high_count = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = network_get_output(config, dataset[i].inputs);
        double delta = 0;
        double target_value = update_value(dataset[i].output);
        if(target_value == 0){
            delta = outputs[0];
        } else {
            delta = (outputs[0] - target_value) / target_value;
        }
        if(delta < 0) {
            delta = -1 * delta;
        }
        // double e = delta * delta;
        if(to_print) {
            if(delta < 1) {
                correct_sign_count += 100;
            }
            if(outputs[0] > 0) {
                high_count += 1;
                if(dataset[i].output > 0) {
                    correct_high_count += 1;
                }
            }
            // printf("Desired outputs: [");
            // printf("%f], ", target_value);

            // printf("real outputs: [");
            // for(uint32_t j=0; j<num_outputs-1; j++) {
            //     printf("%f, ", outputs[j]);
            // }
            // printf("%f]; ", outputs[num_outputs-1]);
            // printf("error = %f;\n", delta);
        }
        error += delta;
    }
    network_clear_outputs(config);  // Clear neurons outputs to not affect the next round
    if(to_print) {
        printf("\tCorrect sign counter = %f percent\n", (double) correct_sign_count / dataset_size);
        printf("\tCorrect high counter = %d of %d\n", correct_high_count, high_count);
        fflush(stdout);
    }
    return error / dataset_size;
}

int run_evolution(network_t *config) {
    double current_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
    size_t counter = 0, print_counter = 0;
    while((current_error > 0.001) && (counter++ < 1000)) {
        network_mutate(config);
        double new_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
        // printf("New error: %f, counter = %lld\n", new_error, counter);
        // fflush(stdout);
        // break;
        if(new_error > current_error) {
            network_rollback(config);
        } else {
            current_error = new_error;
            if(print_counter++ == 10) {
                printf("New error: %f, counter = %lld\n", new_error, counter);
                fflush(stdout);
                print_counter = 0;
            }
        }
    }
    get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 1);
    
    // double after_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
    // if(after_error != current_error) {
    //     printf("ERROR: after_error != current_error: after_error = %f, current_error = %f\n", after_error, current_error);
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}

int main(void) {
    char *n_path = concat_strings(BCKP_DIR_PATH, "/td_micronet");
    srand(time(NULL));
    network_t *unet = calloc(1, sizeof(network_t));
    network_init_micronet(unet, &network_map);
    printf("MicroNet initialised!\n");

    // Restore network 976629 946359
    network_restore(unet, n_path, 1);

    for(int i=0; i<1000; i++) {
        run_evolution(unet);

        // Save network
        network_save(unet, n_path);
    }
    free(n_path);

    return EXIT_SUCCESS;
}