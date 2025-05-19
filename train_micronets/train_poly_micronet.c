#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "network.h"
#include "utils.h"

// Micronet configs:
#include "micro_maps/config_feedback_micronet.h"
#include "micro_maps/config_linear_micronet.h"
#include "micro_maps/config_poly_micronet.h"

// #include "poly_dataset.h"

typedef struct {
    double inputs[3];
    double output[2];
}dataset_item_t;

dataset_item_t summator_dataset[] = {
    {.inputs={0, 0, 0}, .output={0, 0}},
    {.inputs={0, 0, 1}, .output={1, 0}},
    {.inputs={0, 1, 0}, .output={1, 0}},
    {.inputs={0, 1, 1}, .output={0, 1}},
    {.inputs={1, 0, 0}, .output={1, 0}},
    {.inputs={1, 0, 1}, .output={0, 1}},
    {.inputs={1, 1, 0}, .output={0, 1}},
    {.inputs={1, 1, 1}, .output={1, 1}},
};

#define DATASET summator_dataset
#define DATASET_SIZE sizeof_arr(DATASET)
#define NUM_OUTPUTS sizeof_arr(DATASET[0].output)

uint32_t train_micronet_neurons[] = {
    // idx  num_inputs  type 	indices
    // Layer 1:
	   8,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   9,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   10,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   11,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   12,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   13,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   14,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
	   15,	8,			1,		0, 1, 2, 3, 4, 5, 6, 7,
    // Layer 2:
	   16,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   17,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   18,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   19,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   20,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   21,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   22,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	   23,	8,			1,		8, 9, 10, 11, 12, 13, 14, 15,
	// Layer 3:
	   24,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   25,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   26,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   27,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   28,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   29,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   30,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	   31,	8,			1,		16, 17, 18, 19, 20, 21, 22, 23,
	// Layer 4:
	   32,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   33,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   34,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   35,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   36,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   37,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   38,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	   39,	8,			1,		24, 25, 26, 27, 28, 29, 30, 31,
	// Layer 5:
	   40,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   41,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   42,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   43,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   44,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   45,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   46,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	   47,	8,			1,		32, 33, 34, 35, 36, 37, 38, 39,
	// Outputs:
	   48,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   49,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   50,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   51,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   52,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   53,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   54,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
	   55,	8,			1,		40, 41, 42, 43, 44, 45, 46, 47,
};

network_map_t train_micronet_map = {
	.num_inputs = 8,
	.num_neurons = 48,
	.neurons = train_micronet_neurons,
	.num_outputs = 8,
	.output_indices = {48, 49, 50, 51, 52, 53, 54, 55},
};

uint32_t summator_micronet_neurons[] = {
    // idx  num_inputs  type 	indices
	   3,	2,			1,		0, 1,   // XOR
	   4,	2,			1,		0, 1,   // AND
	   5,	2,			1,		3, 2,   // XOR
	   6,	2,			1,		3, 2,   // AND
	   7,	2,			1,		4, 6,   // OR
	   8,	2,			1,		4, 6,   // OR
	   9,	2,			1,		7, 8,   // OR
};

// Inputs: 0 - A
//         1 - B
//         2 - Cin
// Outputs: 0 - S
//          1 - Cout

network_map_t summator_micronet_map = {
	.num_inputs = 3,
	.num_neurons = 7,
	.neurons = summator_micronet_neurons,
	.num_outputs = 2,
	.output_indices = {5, 9},
};

static double get_complex_error(double val1, double val2) {
    double temp = (val1 * val1) + (val2 * val2);
    return sqrt(temp);
}

static double get_local_deviation(double output_value, double target_value) {
    double local_deviation = 0;
    if(target_value == 0) {
        local_deviation = output_value;
    } else {
        local_deviation = (output_value - target_value) / target_value;
    }
    if(local_deviation < 0) {
        local_deviation = -1 * local_deviation;
    }
    return local_deviation;
}

static double get_error(network_t *config, dataset_item_t *dataset, size_t dataset_size, uint32_t num_outputs, uint8_t to_print) {
    double error = 0, max_deviation = 0;
    double *round_errors = calloc(dataset_size, sizeof(double));
    double *round_deviations = calloc(dataset_size, sizeof(double));
    for(uint32_t i=0; i<dataset_size; i++) {
        double *outputs = network_get_output(config, dataset[i].inputs);
        for(uint32_t j=0; j<num_outputs; j++) {
            double local_deviation = get_local_deviation(outputs[j], dataset[i].output[j]);

            round_errors[i] += local_deviation;
            if(local_deviation > round_deviations[i]) {
                round_deviations[i] = local_deviation;
            }
        }
        if(round_deviations[i] > max_deviation) {
            max_deviation = round_deviations[i];
        }
        round_errors[i] /= num_outputs;
        error += round_errors[i];

        if(to_print) {
            printf("Iteration %d:\n", i);
            printf("Desired outputs: [");
            for(uint32_t j=0; j<num_outputs; j++) {
                printf("%.2f ", dataset[i].output[j]);
            }
            printf("]\n");
            printf("   Real outputs: [");
            for(uint32_t j=0; j<num_outputs; j++) {
                printf("%.2f ", outputs[j]);
            }
            printf("]\n");
            printf("error = %f;\n", round_errors[i]);
            fflush(stdout);
        }
    }
    network_clear_outputs(config);  // Clear neurons outputs to not affect the next round
    if(to_print) {
        printf("Round deviations: [");
        for(uint32_t i=0; i<dataset_size; i++) {
            printf("%.2f ", round_deviations[i]);
        }
        printf("]\n");
        printf("Round errors: [");
        for(uint32_t i=0; i<dataset_size; i++) {
            printf("%.2f ", round_errors[i]);
        }
        printf("]\n");
        fflush(stdout);
    }
    // error /= dataset_size;
    free(round_deviations);
    free(round_errors);
    return get_complex_error(max_deviation, error / dataset_size);
    // return max_deviation;
}

int run_evolution(network_t *config) {
    double current_error = get_error(config, DATASET, DATASET_SIZE, NUM_OUTPUTS, 0);
    size_t counter = 0, print_counter = 0;
    while((current_error > 0.001) && (counter++ < 1000)) {
        network_mutate(config);
        double new_error = get_error(config, DATASET, DATASET_SIZE, NUM_OUTPUTS, 0);
        if(new_error > current_error) {
            network_rollback(config);
        } else {
            current_error = new_error;
            if(print_counter++ == 100) {
                printf("New error: %f, counter = %lld\n", new_error, counter);
                fflush(stdout);
                print_counter = 0;
            }
        }
    }
    current_error = get_error(config, DATASET, DATASET_SIZE, NUM_OUTPUTS, 1);
    printf("Final error: %f\n", current_error);
    fflush(stdout);

    return EXIT_SUCCESS;
}

int main(void) {
    char *n_path = concat_strings(BCKP_DIR_PATH, "/train_micronet");
    srand(time(NULL));
    network_t *unet = calloc(1, sizeof(network_t));
    network_init_micronet(unet, &summator_micronet_map);
    printf("MicroNet initialised!\n");

    // Restore network
    // network_restore(unet, n_path, 1);

    printf("Initial error: %f\n", get_error(unet, DATASET, DATASET_SIZE, NUM_OUTPUTS, 0));
    fflush(stdout);

    for(int i=0; i<1000; i++) {
        run_evolution(unet);

        // Save network
        network_save(unet, n_path);
    }
    free(n_path);

    return EXIT_SUCCESS;
}