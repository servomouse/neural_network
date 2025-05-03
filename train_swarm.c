#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network.h"
#include "utils.h"

// Micronet config:
#include "dataset.h"
#include "micro_maps/config_td_network.h"

#define NUM_NETWORKS 100
#define NUM_TO_SAVE  50

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

typedef struct {
    network_t *net;
    double error;
} swarm_item_t;

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
    }
    return error / dataset_size;
}

int run_evolution(network_t *config) {
    double current_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
    while(counter++ < 1000) {
        network_mutate(config);
        double new_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 1, 0);
        if(new_error > current_error) {
            network_rollback(config);
        } else {
            current_error = new_error;
        }
    }

    return EXIT_SUCCESS;
}

#define BUF_SIZE 128

swarm_item_t swarm_init(uint32_t swarm_size) {
    swarm_item_t *swarm = calloc(swarm_size, sizeof(swarm_item_t));
    for(uint32_t i=0; i<swarm_size; i++) {
        swarm[i].net = calloc(1, sizeof(network_t));
        network_init_micronet(swarm[i].net, &network_map);
        swarm[i].error = 0;
    }
    return swarm;
}

void swarm_run_evolution(swarm_t *swarm, uint32_t num_rounds) {
    for(uint32_t i=0; i<num_rounds; i++) {
        run_evolution(swarm[i].net);
    }
}

void swarm_sort(swarm_t *swarm, uint32_t swarm_size) {
    uint8_t sorted;
    uint32_t counter = 0;
    while(1) {
        sorted = 1;
        for(uint32_t i=1; i<swarm_size; i++) {
            if(swarm[i].error < swarm[i-1].error) {
                double error = swarm[i].error;
                network_t *net = swarm[i].net;
                swarm[i].net = swarm[i-1].net;
                swarm[i].error = swarm[i-1].error;
                swarm[i-1].net = net;
                swarm[i-1].error = error;
                sorted = 0;
            }
        }
        if(sorted) {
            break;
        }
        if(counter++ > swarm_size) {
            printf("Error: Sorting the swarm gone bad :(\n");
            exit(1);
        }
    }
}

void swarm_save_update(network_t swarm, uint32_t swarm_size, uint32_t num_to_save) {
    char buf[BUF_SIZE];
    // Save networks on disk:
    for(uint32_t i=0; i<num_to_save; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/swarm/network_%d", i);
        char *n_path = concat_strings(path, buf);
        network_save(swarm[i].net, n_path);
        free(n_path);
    }
    // Overwrite the remaining networks
    for(uint32_t n=num_to_save; n<swatm_size; n++) {
        clear_buffer(buf, BUF_SIZE);
        uint32_t idx = n%num_to_save;
        snprintf(buf, BUF_SIZE, "/swarm/network_%d", idx);
        char *n_path = concat_strings(path, buf);
        network_restore(swarm[n].net, n_path, 1);
        free(n_path);
    }
}

void swarm_restore(network_t swarm, uint32_t num_to_save) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<num_to_save; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/swarm/network_%d", i);
        char *n_path = concat_strings(path, buf);
        network_restore(swarm[i].net, n_path, 1);
        free(n_path);
    }
}

void swarm_print_results(network_t swarm) {
    get_error(swarm[0].net, btc_dataset, sizeof_arr(btc_dataset), 1, 1);
}

int main(void) {
    network_t *swarm = swarm_init(NUM_NETWORKS);
    // swarm_restore(swarm, NUM_TO_SAVE);
    printf("Swarm initialised!\n");
    for(uint32_t i=0; i<100; i++) {
        swarm_run_evolution(swarm, 10);
        swarm_sort(swarm);
        swarm_save_update(swarm, NUM_NETWORKS, NUM_TO_SAVE);
        swarm_print_results(swarm);
    }
    return EXIT_SUCCESS;
}
