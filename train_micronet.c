#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"
#include "micronet.h"
#include "utils.h"

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

// Neuron types:
// 0 - linear
// 1 - polynomial

uint32_t linear_neurons[] = {
    // idx  num_inputs  type indices
       4,   4,          0,   0, 1, 2, 3,
       5,   4,          0,   0, 1, 2, 3,
       6,   4,          0,   0, 1, 2, 3,
       7,   4,          0,   0, 1, 2, 3,
       8,   4,          0,   4, 5, 6, 7,
};

micronet_map_t linear_micronet_map = {
    .num_inputs = 4,
    .num_neurons = 5,
    .net_size = 9,
    .neurons = linear_neurons,
    .num_outputs = 1,
    .output_indices = {8},
};

double linear_coeffs[5][5] = {  // Plus offset
    {-0.60, -0.75, -0.10, 0.40, 0.0},
    {0.40, -0.70, 0.35, 0.20, 0.70},
    {0.70, 0.60, -0.50, 0.60, 0.60},
    {0.0, -0.25, 0.70, -0.60, 0.60},
    {0.75, 0.15, 0.75, 0.60, -0.60},
};

typedef struct {
    double inputs[4];
    double outputs[1];
} dataset_entry_t;

dataset_entry_t linear_dataset[] = {
    {.inputs = {0.199320, -0.668140, 0.777210, 0.263040}, .outputs = {0.537650}},
    {.inputs = {0.288430, -0.081390, -0.556510, 0.019750}, .outputs = {0.347243}},
    {.inputs = {0.645860, -0.719660, 0.106780, -0.368080}, .outputs = {0.405290}},
    {.inputs = {-0.539540, 0.511760, -0.500110, -0.149140}, .outputs = {-0.019801}},
    {.inputs = {0.627920, 0.477830, 0.372300, -0.605580}, .outputs = {-0.073561}},
    {.inputs = {0.097260, 0.023590, -0.888120, -0.670030}, .outputs = {0.018065}},
    {.inputs = {0.972110, -0.078710, 0.152930, -0.898070}, .outputs = {-0.060608}},
    {.inputs = {-0.322370, -0.381210, -0.019070, -0.233310}, .outputs = {0.313155}},
    {.inputs = {0.507070, 0.851250, 0.547840, 0.996400}, .outputs = {-0.090780}},
    {.inputs = {0.858700, -0.821960, 0.910280, -0.584640}, .outputs = {-0.091233}},
    {.inputs = {0.477830, -0.490830, 0.902340, 0.830680}, .outputs = {0.821575}},
    {.inputs = {-0.985660, 0.680960, -0.343490, -0.079930}, .outputs = {-0.109616}},
    {.inputs = {-0.790400, -0.854120, 0.784660, -0.060950}, .outputs = {0.229011}},
    {.inputs = {0.024690, 0.838620, -0.968810, 0.010280}, .outputs = {-0.465611}},
    {.inputs = {-0.977840, -0.721120, 0.212560, 0.557420}, .outputs = {0.439960}},
    {.inputs = {0.315040, 0.976870, -0.747490, 0.445910}, .outputs = {-0.616961}},
};

static double get_error(micro_network_t *net, dataset_entry_t *dataset, size_t dataset_size, uint32_t num_outputs) {
    // Works only with single output networks
    double error = 0;
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = micronet_get_output(net, dataset[i].inputs);
        double e = 0.0;
        for(uint32_t j=0; j<num_outputs; j++) {
            double delta = dataset[i].outputs[j] - outputs[j];
            e += delta * delta;
        }

        printf("Desired outputs: [");
        for(uint32_t j=0; j<num_outputs-1; j++) {
            printf("%f, ", dataset[i].outputs[j]);
        }
        printf("%f], ", dataset[i].outputs[num_outputs-1]);

        printf("real outputs: [");
        for(uint32_t j=0; j<num_outputs-1; j++) {
            printf("%f, ", outputs[j]);
        }
        printf("%f];\n", outputs[num_outputs-1]);

        error += e;
    }
    return error / dataset_size;
}

/*
- global error
- feedback error
- neuron output
- coefficient value
- input value
- c_net stash
- f_net stash
- global stash
*/

uint32_t smart_neurons[] = {
    // idx  num_inputs  type indices
       8,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
       9,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
      10,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
      11,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,

      12,   4,          1,   8, 9, 10, 11,
};

micronet_map_t smart_micronet_map = {
    .num_inputs = 8,
    .num_neurons = 5,
    .net_size = 13,
    .neurons = smart_neurons,
    .num_outputs = 1,
    .output_indices = {12},
};

int evolution(void) {
    srand(time(NULL));
    micro_network_t target_net;
    micronet_init(&target_net, &linear_micronet_map, NULL);
    for(uint32_t i=0; i<5; i++) {
        // printf("{");
        for(uint32_t j=0; j<5; j++) {
            // double coeff = random_double(-0.75, 0.75);
            // printf("%f, ", coeff);
            neuron_set_coeff(&target_net.neurons[i], j, linear_coeffs[i][j]);
        }
        // printf("},\n");
    }
    printf("Target microNet initialised!\n");
    micro_network_t c_micronet;
    micronet_init(&c_micronet, &smart_micronet_map, NULL);
    printf("C microNet initialised!\n");
    micro_network_t f_micronet;
    micronet_init(&f_micronet, &smart_micronet_map, NULL);
    printf("F microNet initialised!\n");

    // // Dataset generator
    // for(uint32_t i=0; i<16; i++) {
    //     double inputs[4] = {
    //         random_double(-1, 1),
    //         random_double(-1, 1),
    //         random_double(-1, 1),
    //         random_double(-1, 1),
    //     };
    //     double *outputs = micronet_get_output(&target_net, inputs);
    //     printf("{.inputs = {%f, %f, %f, %f}, .outputs = {%f}},\n", inputs[0], inputs[1], inputs[2], inputs[3], outputs[0]);
    // }

    double init_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    printf("Init error: %f\n", init_error);
    // double current_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    // printf("Current error: %f\n", current_error);

    return 0;
}

// #define TESTS_MODE

int main(void) {
    #ifdef TESTS_MODE
    srand(time(NULL));
    micro_network_t config;
    micronet_init(&config, &micronet_map, NULL);
    printf("MicroNet initialised!\n");
    // get_error(&config, dataset, sizeof_arr(dataset), 1, 0);

    if(test_func(test_basic_functions,      &config, "basic functions") ||
       test_func(test_multiple_mutations,   &config, "multiple mutations")||
       test_func(test_evolution,            &config, "evolution")) {
        return EXIT_FAILURE;
    }
    #else
    evolution();
    #endif
    return EXIT_SUCCESS;
}