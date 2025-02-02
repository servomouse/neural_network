#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "network_new.h"

#define DATASET_NUM_OUTPUTS 1

typedef struct {
    double inputs[4];
    double output[DATASET_NUM_OUTPUTS];
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
//     {.inputs = {0.1, 0.2, 0.3, 0.4}, .output = {0.1234}},
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

// dataset_entry_t dataset[] = {
//     {.inputs = {-1, 1.0, -1, 1.0}, .output = {1.0}},
//     // {.inputs = {-1, 1.0, 0.9, 0.8}, .output = {-0.3732}},
//     // {.inputs = {-0.9, -0.6, 0.9, 1.0}, .output = {0.1148}},
//     // {.inputs = {0.9, 0.8, -1, 0.7}, .output = {-0.128}},
//     // {.inputs = {0.9, -0.8, 1.0, -0.9}, .output = {0.2721}},
//     // {.inputs = {-1, -0.8, -0.7, 1.0}, .output = {-0.1756}},
//     // {.inputs = {0.7, -0.8, 1.0, 1.0}, .output = {-0.1756}},
//     // {.inputs = {0.7, 1.0, 0.8, -0.9}, .output = {-0.128}},
//     // {.inputs = {0.8, -1, 0.9, -1}, .output = {0.3732}},
//     // {.inputs = {1.0, -1, 0.7, 0.8}, .output = {-0.1756}},
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
    //  idx num_inputs  output_idx  input_indices
        4,  4,          NOT_OUTPUT, 0, 1, 2, 3,
        5,  4,          NOT_OUTPUT, 0, 1, 2, 3,
        6,  4,          NOT_OUTPUT, 0, 1, 2, 3,
        7,  4,          NOT_OUTPUT, 0, 1, 2, 3,
        8,  4,          0,          4, 5, 6, 7,
        // {.idx = 7, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 8, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 9, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 10, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 11, .num_inputs = 6, .indices = {0, 1, 2, 3, 4, 5, 0, 0}},
        // {.idx = 12, .num_inputs = 6, .indices = {6, 7, 8, 9, 10, 11, 0, 0}},
    },
};

char *stages[] = {
    "before",
    "after"
};

double get_average_error(uint32_t num_outputs, double *errors) {
    double average_error = 0;
    for(uint32_t j=0; j<num_outputs; j++) {
        average_error += errors[j];
    }
    return average_error / num_outputs;
}

double train_network(network_t *config, uint8_t to_print) {
    // double error = 0;
    double local_errors[DATASET_NUM_OUTPUTS] = {0.0};
    double global_errors[sizeof_arr(dataset)] = {0.0};
    uint32_t dataset_size = sizeof_arr(dataset);
    uint32_t num_outputs = DATASET_NUM_OUTPUTS;
    network_reset_activations(config);
    for(size_t i=0; i<sizeof_arr(dataset); i++) {
        double *outputs = network_get_outputs(config, dataset[i].inputs, to_print);

        for(uint32_t j=0; j<num_outputs; j++) {
            double diff = dataset[i].output[j] - outputs[j];
            local_errors[j] = diff * diff;
        }
        global_errors[i] = get_average_error(local_errors, num_outputs);

        network_set_output_errors(config, local_errors);
        network_set_global_error(config, global_errors[i]);
        network_generate_feedbacks(config);
        network_update_weights(config);

        if(to_print) {
            printf("\"global_error\": %f\n},\n", global_errors[i]);
            // printf("Desired outputs: [");
            // for(uint32_t j=0; j<num_outputs; j++) {
            //     printf("%f, ", dataset[i].output[j]);
            // }
            // printf("]; real outputs: [");
            // for(uint32_t j=0; j<num_outputs; j++) {
            //     printf("%f, ", outputs[j]);
            // }
            // printf("]\n");
        }
        // error += e;
    }
    
    // for(uint32_t j=0; j<num_outputs; j++) {
    //     global_errors[j] / dataset_size;
    // }
    return get_average_error(global_errors, dataset_size);
}

uint32_t counter = 0;

double get_error(network_t *config, uint32_t num_outputs, dataset_entry_t *dataset, size_t dataset_size, uint8_t to_print) {
    // double error = 0;
    double *local_errors = calloc(num_outputs, sizeof(double));
    double *global_errors = calloc(dataset_size, sizeof(double));
    network_reset_counters(config);
    counter = 0;
    for(size_t i=0; i<dataset_size; i++) {
        if(to_print) {
            printf("\"stage_%lld_%s: {\n", i, stages[counter]);
        }
        double *outputs = network_get_outputs(config, dataset[i].inputs, to_print);
        // double e = 0;

        for(uint32_t j=0; j<num_outputs; j++) {
            double diff = dataset[i].output[j] - outputs[j];
            local_errors[j] = diff * diff;
        }
        network_set_local_errors(config, local_errors, i);
        global_errors[i] = get_average_error(local_errors, num_outputs);
        network_set_global_error(config, global_errors[i], i);

        if(to_print) {
            printf("\"global_error\": %f\n},\n", global_errors[i]);
            // printf("Desired outputs: [");
            // for(uint32_t j=0; j<num_outputs; j++) {
            //     printf("%f, ", dataset[i].output[j]);
            // }
            // printf("]; real outputs: [");
            // for(uint32_t j=0; j<num_outputs; j++) {
            //     printf("%f, ", outputs[j]);
            // }
            // printf("]\n");
        }
        // error += e;
    }
    counter ++;
    
    // for(uint32_t j=0; j<num_outputs; j++) {
    //     global_errors[j] / dataset_size;
    // }
    return get_average_error(global_errors, dataset_size);
}

int test_mutations(network_t *config) {
    double init_error = get_error(config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
    printf("Error before mutation: %f\n", init_error);

    network_mutate(config);
    double current_error = get_error(config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
    printf("Error after mutation: %f\n", current_error);

    network_rollback(config);
    double final_error = get_error(config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
    printf("Error after rollback: %f\n", final_error);

    if(final_error != init_error) {
        printf("Error: final_error != int_error!!: final_error = %f, init_error = %f\n", final_error, init_error);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(void) {
    srand(time(NULL));
    network_t config;
    network_init(&config, &network_map, sizeof_arr(dataset));
    printf("Network initialised!\n");

    double init_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
    double current_error = init_error;
    printf("Init error: %f\n", current_error);
    network_stash_neurons(&config);

    size_t counter = 0;
    double new_error = 0;
    while((current_error > 0.001) && (counter++ < 1000)) {
        network_mutate_micronet(&config);
        // printf("Updating neurons\n");
        // for(size_t i=0; i<10; i++) {
            network_update_neurons(&config);
        // }
        new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
        network_rollback_neurons(&config);
        if(new_error > current_error) {
            network_rollback_micronet(&config);
        } else {
            if(new_error < current_error) {
                printf("New error: %f\n", new_error);
                fflush(stdout);
            }
            current_error = new_error;
        }
        new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
        if(new_error != init_error) {
            printf("New error != init_error after rollback!: new_error = %f, init_error = %f\n", new_error, init_error);
            return 1;
        }
    }
    
    printf("Final error: %f, counter = %lld\n", current_error, counter);
    current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);   // Just print values
    printf("Final error: %f, counter = %lld\n", current_error, counter);

    network_save_data(&config, "network_backup.h");

    return 0;
}

// // #define USE_BACKUP

// int main(void) {
//     srand(time(NULL));
//     network_t config;
//     #ifdef USE_BACKUP
//     network_init(&config, &backup_map, sizeof_arr(dataset), backup_coeffs); // Restore network (recompilation required)
//     #else
//     network_init(&config, &network_map, sizeof_arr(dataset), NULL);
//     #endif
//     printf("Network initialised!\n");

//     double init_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
//     double current_error = init_error;
//     printf("Init error: %f\n", current_error);

//     size_t counter = 0;
//     double new_error = 0;
//     while((current_error > 0.001) && (counter++ < 100000)) {
//         network_mutate(&config);
//         new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
//         if(new_error > current_error) {
//             network_rollback(&config);
//             new_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);
//             if(new_error != current_error) {
//                 printf("New error != current error after rollback!: new_error = %f, current_error = %f\n", new_error, current_error);
//                 return 1;
//             }
//         } else {
//             if(new_error < current_error) {
//                 printf("New error: %f\n", new_error);
//             }
//             current_error = new_error;
//         }
//     }
    
//     printf("Final error: %f, counter = %lld\n", current_error, counter);
//     current_error = get_error(&config, network_map.num_outputs, dataset, sizeof_arr(dataset), 0);   // Just print values
//     printf("Final error: %f, counter = %lld\n", current_error, counter);

//     network_save_data(&config, "network_backup.h");

//     return 0;
// }