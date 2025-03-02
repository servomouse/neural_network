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

        // printf("Desired outputs: [");
        // for(uint32_t j=0; j<num_outputs-1; j++) {
        //     printf("%f, ", dataset[i].outputs[j]);
        // }
        // printf("%f], ", dataset[i].outputs[num_outputs-1]);

        // printf("real outputs: [");
        // for(uint32_t j=0; j<num_outputs-1; j++) {
        //     printf("%f, ", outputs[j]);
        // }
        // printf("%f];\n", outputs[num_outputs-1]);

        error += e;
    }
    return error / dataset_size;
}

static void train_net(micro_network_t *target_net, micro_network_t *c_net, micro_network_t *f_net, dataset_entry_t *dataset, size_t dataset_size, uint32_t num_outputs) {
    // Works only with single output networks
    // double error = 0;
    double *errors = calloc(num_outputs, sizeof(double));
    for(size_t i=0; i<dataset_size; i++) {
        double *outputs = micronet_get_output(target_net, dataset[i].inputs);
        double e = 0.0;
        for(uint32_t j=0; j<num_outputs; j++) {
            double delta = dataset[i].outputs[j] - outputs[j];
            errors[j] = delta * delta;
            e += errors[j];
        }
        micronet_set_global_error(target_net, e/num_outputs, errors);

        
        // double new_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
        // double delta = new_error - init_error;
        // double e_val = delta * delta;
        // // #error "Update f_micronet (5 inputs and 2 outputs)"
        micronet_update_feedbacks(target_net, f_net);
        micronet_update_coeffs(target_net, c_net);

        // printf("Desired outputs: [");
        // for(uint32_t j=0; j<num_outputs-1; j++) {
        //     printf("%f, ", dataset[i].outputs[j]);
        // }
        // printf("%f], ", dataset[i].outputs[num_outputs-1]);

        // printf("real outputs: [");
        // for(uint32_t j=0; j<num_outputs-1; j++) {
        //     printf("%f, ", outputs[j]);
        // }
        // printf("%f];\n", outputs[num_outputs-1]);

        // error += e;
    }
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

// uint32_t smart_neurons[] = {
//     // idx  num_inputs  type indices
//        8,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
//        9,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
//       10,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,
//       11,   8,          1,   0, 1, 2, 3, 4, 5, 6, 7,

//       12,   4,          1,   8, 9, 10, 11,
// };

uint32_t smart_neurons[] = {
    // idx  num_inputs  type indices
       5,   5,          1,   0, 1, 2, 3, 4,
       6,   5,          1,   0, 1, 2, 3, 4,
       7,   5,          1,   0, 1, 2, 3, 4,
       8,   5,          1,   0, 1, 2, 3, 4,
       9,   5,          1,   0, 1, 2, 3, 4,

    //   10,   5,          1,   5, 6, 7, 8, 9,
    //   11,   5,          1,   5, 6, 7, 8, 9,
    //   12,   5,          1,   5, 6, 7, 8, 9,
    //   13,   5,          1,   5, 6, 7, 8, 9,
    //   14,   5,          1,   5, 6, 7, 8, 9,

    //   15,   6,          1,   10, 11, 12, 13, 14, 19,
    //   16,   6,          1,   10, 11, 12, 13, 14, 15,
    //   17,   6,          1,   10, 11, 12, 13, 14, 16,
    //   18,   6,          1,   10, 11, 12, 13, 14, 17,
    //   19,   6,          1,   10, 11, 12, 13, 14, 18,

      10,   5,          1,   5, 6, 7, 8, 9,
      11,   5,          1,   5, 6, 7, 8, 9,
};

micronet_map_t smart_micronet_map = {
    .num_inputs = 5,
    .num_neurons = 7,
    .net_size = 12,
    .neurons = smart_neurons,
    .num_outputs = 2,
    .output_indices = {10, 11},
};

void init_coeffs(micro_network_t *net) {
    for(uint32_t i=0; i<5; i++) {
        // printf("{");
        for(uint32_t j=0; j<5; j++) {
            // double coeff = random_double(-0.75, 0.75);
            // printf("%f, ", coeff);
            neuron_set_coeff(&net->neurons[i], j, linear_coeffs[i][j]);
        }
        // printf("},\n");
    }
}

static double get_delta(micro_network_t *target_net, micro_network_t *c_net, micro_network_t *f_net) {
    double init_error = get_error(target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    micronet_clear_feedbacks(target_net);
    train_net(target_net, c_net, f_net, linear_dataset, sizeof_arr(linear_dataset), 1);

    double final_error = get_error(target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    if(init_error == 0.0) {
        return 0;
    }
    double delta;
    if(init_error > final_error) {
        delta = (init_error - final_error) / init_error;
    } else {
        delta = -1 * (final_error - init_error) / final_error;
    }
    // printf("init_error: %f, final_error: %f, delta: %f\n", init_error, final_error, delta);
    return delta;
}

static double get_average_delta(micro_network_t *target_net, micro_network_t *c_net, micro_network_t *f_net, double coeff_inc) {
    double init_average_delta = 0.0;
    uint32_t average_delta_counter = 0;

    for(uint32_t n=0; n<5; n++) {
        for(uint32_t c=0; c<5; c++) {
            init_coeffs(target_net);
            neuron_set_coeff(&target_net->neurons[n], c, coeff_inc);

            init_average_delta += get_delta(target_net, c_net, f_net);
            average_delta_counter++;
        }
    }

    return init_average_delta / average_delta_counter;
}

int evolution(void) {
    srand(time(NULL));
    micro_network_t target_net = {0};
    micronet_init(&target_net, &linear_micronet_map);
    init_coeffs(&target_net);
    printf("Target microNet initialised!\n");
    micro_network_t c_micronet = {0};
    micronet_init(&c_micronet, &smart_micronet_map);
    printf("C microNet initialised!\n");
    micro_network_t f_micronet = {0};
    micronet_init(&f_micronet, &smart_micronet_map);
    printf("F microNet initialised!\n");
    
    // char *target_net_path = concat_strings(BCKP_DIR_PATH, "/target_unet");
    char *c_net_path = concat_strings(BCKP_DIR_PATH, "/c_unet");
    char *f_net_path = concat_strings(BCKP_DIR_PATH, "/f_unet");
    micronet_restore(&c_micronet, c_net_path);
    micronet_restore(&f_micronet, f_net_path);

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
    // char *n_path = concat_strings(BCKP_DIR_PATH, "/target_unet");
    // micronet_save(&target_net, n_path);
    
    // for(uint32_t i=0; i<5; i++) {
    //     for(uint32_t j=0; j<5; j++) {
    //         neuron_set_coeff(&target_net.neurons[i], j, 0.1);
    //     }
    // }
    // init_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    // printf("Error after save: %f\n", init_error);
    // micronet_restore(&target_net, n_path);
    // init_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    // printf("Error after restore: %f\n", init_error);
    // return 0;

    double delta_sum = 0;
    uint32_t delta_counter = 0;
    uint32_t pos_delta_counter = 0;
    uint32_t pos_new_delta_counter = 0;
    uint32_t delta_improved_counter = 0;

    for(uint32_t i=0; i<10000; i++) {
        // printf("Round %d:\n", i);

        // uint32_t coords[2] = {
        //     random_int(0, 5),
        //     random_int(0, 5)
        // };
        double coeff_inc = random_double(-0.5, 0.5);

        double init_average_delta = get_average_delta(&target_net, &c_micronet, &f_micronet, coeff_inc);

        delta_sum += init_average_delta;
        delta_counter ++;
        if(init_average_delta > 0) {
            pos_delta_counter ++;
        }

        micronet_mutate(&c_micronet);
        micronet_mutate(&f_micronet);

        double after_average_delta = get_average_delta(&target_net, &c_micronet, &f_micronet, coeff_inc);

        if(after_average_delta < init_average_delta) {
            micronet_rollback(&f_micronet);
            micronet_rollback(&c_micronet);
        }
        if(after_average_delta > init_average_delta) {
            delta_improved_counter ++;
        }
        if(i%1000 == 0) {
            printf("Init delta: %f, new_delta: %f, average delta: %f\n", init_average_delta, after_average_delta, delta_sum/delta_counter);
            fflush(stdout);
            if(i > 0) {
                micronet_save(&c_micronet, c_net_path);
                micronet_save(&f_micronet, f_net_path);
            }
        }
        if(after_average_delta > 0) {
            pos_new_delta_counter ++;
        }
    }
    // double current_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    // printf("Current error: %f\n", current_error);
    printf("Average_delta: %f, delta_counter = %d\n", delta_sum/delta_counter, delta_counter);
    printf("Positive delta counter: %d\n", pos_delta_counter);
    printf("Positive new delta counter: %d\n", pos_new_delta_counter);
    printf("Improved delta counter: %d\n", delta_improved_counter);
    printf("c_micronet coeffs:\n");
    micronet_print_coeffs(&c_micronet);
    printf("f_micronet coeffs:\n");
    micronet_print_coeffs(&f_micronet);

    // remove_folders(BCKP_DIR_PATH);
    micronet_save(&c_micronet, c_net_path);
    micronet_save(&f_micronet, f_net_path);

    free(c_net_path);
    free(f_net_path);

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