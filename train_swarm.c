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

#define BUF_SIZE 128

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

typedef struct {
    network_t *net;
    double error;
} swarm_item_t;

double update_value(double value) {
    double temp = value * 100;
    if(temp > 1) {temp = 1;}
    else if(temp < -1) {temp = -1;}
    return temp;
}

static double get_error(network_t *config, dataset_item_t *dataset, size_t dataset_size, uint8_t to_print) {
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
    double current_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 0);
    uint32_t counter = 0;
    while(counter++ < 1000) {
        network_mutate(config);
        double new_error = get_error(config, btc_dataset, sizeof_arr(btc_dataset), 0);
        if(new_error > current_error) {
            network_rollback(config);
        } else {
            current_error = new_error;
        }
    }

    return EXIT_SUCCESS;
}

swarm_item_t *swarm_init(uint32_t swarm_size) {
    swarm_item_t *swarm = calloc(swarm_size, sizeof(swarm_item_t));
    for(uint32_t i=0; i<swarm_size; i++) {
        swarm[i].net = calloc(1, sizeof(network_t));
        network_init_micronet(swarm[i].net, &network_map);
        swarm[i].error = 0;
    }
    return swarm;
}

void swarm_run_evolution(swarm_item_t *swarm, uint32_t num_rounds) {
    for(uint32_t i=0; i<num_rounds; i++) {
        run_evolution(swarm[i].net);
    }
}

void swarm_sort(swarm_item_t *swarm, uint32_t swarm_size) {
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

void swarm_save_update(swarm_item_t *swarm, uint32_t swarm_size, uint32_t num_to_save, const char* path) {
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
    for(uint32_t n=num_to_save; n<swarm_size; n++) {
        clear_buffer(buf, BUF_SIZE);
        uint32_t idx = n%num_to_save;
        snprintf(buf, BUF_SIZE, "/swarm/network_%d", idx);
        char *n_path = concat_strings(path, buf);
        network_restore(swarm[n].net, n_path, 1);
        free(n_path);
    }
}

void swarm_restore(swarm_item_t *swarm, uint32_t num_to_save, const char* path) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<num_to_save; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/swarm/network_%d", i);
        char *n_path = concat_strings(path, buf);
        network_restore(swarm[i].net, n_path, 1);
        free(n_path);
    }
}

void swarm_print_results(swarm_item_t *swarm) {
    get_error(swarm[0].net, btc_dataset, sizeof_arr(btc_dataset), 1);
}

int main(void) {
    srand(time(NULL));
    char *n_path = concat_strings(BCKP_DIR_PATH, "/td_micronet");
    swarm_item_t *swarm = swarm_init(NUM_NETWORKS);
    // swarm_restore(swarm, NUM_TO_SAVE, n_path);
    printf("Swarm initialised!\n");
    for(uint32_t i=0; i<100; i++) {
        swarm_run_evolution(swarm, 10);
        swarm_sort(swarm, NUM_NETWORKS);
        swarm_save_update(swarm, NUM_NETWORKS, NUM_TO_SAVE, n_path);
        swarm_print_results(swarm);
    }
    free(n_path);
    return EXIT_SUCCESS;
}
