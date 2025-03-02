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

uint32_t poly_neurons[] = {
    // idx  num_inputs  type indices
       4,   4,          1,   0, 1, 2, 3,
       5,   4,          1,   0, 1, 2, 3,
       6,   4,          1,   0, 1, 2, 3,
       7,   4,          1,   0, 1, 2, 3,
       8,   4,          1,   4, 5, 6, 7,
};

micronet_map_t poly_micronet_map = {
    .num_inputs = 4,
    .num_neurons = 5,
    .net_size = 9,
    .neurons = poly_neurons,
    .num_outputs = 1,
    .output_indices = {8},
};

double poly_coeffs[5][16] = {  // Plus offset
    {-0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125, 0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 0.999},
    {0.875, 0.999, -0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125, 0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750},
    {0.625, 0.750, 0.875, 0.999, -0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125, 0.000, 0.125, 0.250, 0.375, 0.500},
    {0.375, 0.500, 0.625, 0.750, 0.875, 0.999, -0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125, 0.000, 0.125, 0.250},
    {0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 0.999, -0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125},
    // {-0.60, -0.75, -0.10, 0.40, 0.0},
    // {0.40, -0.70, 0.35, 0.20, 0.70},
    // {0.70, 0.60, -0.50, 0.60, 0.60},
    // {0.0, -0.25, 0.70, -0.60, 0.60},
    // {0.75, 0.15, 0.75, 0.60, -0.60},
};

typedef struct {
    double inputs[4];
    double outputs[1];
} dataset_entry_t;

dataset_entry_t poly_dataset[] = {
    {.inputs = {0.246620, -0.617300, -0.212990, 0.525990}, .outputs = {-0.698981}},
    {.inputs = {0.494610, 0.271100, -0.803890, 0.160310}, .outputs = {-0.636200}},
    {.inputs = {-0.239420, -0.154210, -0.838130, 0.771170}, .outputs = {-0.426605}},
    {.inputs = {-0.616570, -0.241000, -0.136080, 0.978640}, .outputs = {-0.342669}},
    {.inputs = {-0.720760, 0.006620, -0.082490, 0.623890}, .outputs = {-0.234155}},
    {.inputs = {-0.645920, -0.763660, -0.683160, 0.806630}, .outputs = {-0.215539}},
    {.inputs = {-0.569320, -0.744190, -0.485270, 0.432420}, .outputs = {-0.152892}},
    {.inputs = {-0.548940, 0.105990, 0.216220, 0.613030}, .outputs = {-0.150889}},
    {.inputs = {-0.720270, 0.001560, 0.520740, 0.318580}, .outputs = {-0.105555}},
    {.inputs = {0.412210, -0.822080, 0.159760, 0.227940}, .outputs = {-0.072836}},
    {.inputs = {-0.781300, -0.845760, -0.726190, -0.934930}, .outputs = {-0.024376}},

    {.inputs = {-0.685840, -0.285130, -0.471540, 0.059600}, .outputs = {0.024409}},
    {.inputs = {-0.821960, -0.428880, 0.933650, -0.030000}, .outputs = {0.069074}},
    {.inputs = {0.259740, -0.955990, -0.185890, -0.928530}, .outputs = {0.142579}},
    {.inputs = {0.955930, 0.826470, -0.373460, 0.838070}, .outputs = {0.167440}},
    {.inputs = {-0.808530, -0.248510, -0.798390, -0.676810}, .outputs = {0.177894}},
    {.inputs = {-0.590140, 0.664600, -0.042700, -0.025970}, .outputs = {0.271267}},
    {.inputs = {0.042880, -0.931760, 0.529770, 0.831780}, .outputs = {0.370550}},
    {.inputs = {-0.458970, -0.894470, 0.712940, -0.274640}, .outputs = {0.456674}},
    {.inputs = {-0.715450, 0.589280, -0.586960, -0.710320}, .outputs = {0.576593}},
    {.inputs = {-0.678950, 0.354780, -0.406900, -0.985050}, .outputs = {0.597280}},
    {.inputs = {-0.310160, 0.709520, -0.992430, -0.251690}, .outputs = {0.813158}},
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

uint32_t smart_neurons[] = {
    // idx  num_inputs  type indices
       5,   5,          1,   0, 1, 2, 3, 4,
       6,   5,          1,   0, 1, 2, 3, 4,
       7,   5,          1,   0, 1, 2, 3, 4,
       8,   5,          1,   0, 1, 2, 3, 4,
       9,   5,          1,   0, 1, 2, 3, 4,

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
        for(uint32_t j=0; j<16; j++) {
            neuron_set_coeff(&net->neurons[i], j, poly_coeffs[i][j]);
        }
    }
}

// static double get_delta(micro_network_t *target_net, micro_network_t *c_net, micro_network_t *f_net) {
//     double init_error = get_error(target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
//     micronet_clear_feedbacks(target_net);
//     train_net(target_net, c_net, f_net, linear_dataset, sizeof_arr(linear_dataset), 1);

//     double final_error = get_error(target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
//     if(init_error == 0.0) {
//         return 0;
//     }
//     double delta;
//     if(init_error > final_error) {
//         delta = (init_error - final_error) / init_error;
//     } else {
//         delta = -1 * (final_error - init_error) / final_error;
//     }
//     // printf("init_error: %f, final_error: %f, delta: %f\n", init_error, final_error, delta);
//     return delta;
// }

// static double get_average_delta(micro_network_t *target_net, micro_network_t *c_net, micro_network_t *f_net, double coeff_inc) {
//     double init_average_delta = 0.0;
//     uint32_t average_delta_counter = 0;

//     for(uint32_t n=0; n<5; n++) {
//         for(uint32_t c=0; c<5; c++) {
//             init_coeffs(target_net);
//             neuron_set_coeff(&target_net->neurons[n], c, coeff_inc);

//             init_average_delta += get_delta(target_net, c_net, f_net);
//             average_delta_counter++;
//         }
//     }

//     return init_average_delta / average_delta_counter;
// }

int main(void) {
    srand(time(NULL));
    micro_network_t target_net = {0};
    micronet_init(&target_net, &poly_micronet_map);
    init_coeffs(&target_net);
    micronet_print_coeffs(&target_net);
    // printf("Target microNet initialised!\n");
    // micro_network_t c_micronet = {0};
    // micronet_init(&c_micronet, &smart_micronet_map);
    // printf("C microNet initialised!\n");
    // micro_network_t f_micronet = {0};
    // micronet_init(&f_micronet, &smart_micronet_map);
    // printf("F microNet initialised!\n");

    // char *c_net_path = BCKP_DIR_PATH "/poly/c_unet";
    // char *f_net_path = BCKP_DIR_PATH "/poly/f_unet";
    // micronet_restore(&c_micronet, c_net_path);
    // micronet_restore(&f_micronet, f_net_path);

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
    // return 0;

    double init_error = get_error(&target_net, poly_dataset, sizeof_arr(poly_dataset), 1);
    printf("Init error: %f\n", init_error);
    return 0;

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

    // double delta_sum = 0;
    // uint32_t delta_counter = 0;
    // uint32_t pos_delta_counter = 0;
    // uint32_t pos_new_delta_counter = 0;
    // uint32_t delta_improved_counter = 0;

    // for(uint32_t i=0; i<10000; i++) {
    //     // printf("Round %d:\n", i);

    //     // uint32_t coords[2] = {
    //     //     random_int(0, 5),
    //     //     random_int(0, 5)
    //     // };
    //     double coeff_inc = random_double(-0.5, 0.5);

    //     double init_average_delta = get_average_delta(&target_net, &c_micronet, &f_micronet, coeff_inc);

    //     delta_sum += init_average_delta;
    //     delta_counter ++;
    //     if(init_average_delta > 0) {
    //         pos_delta_counter ++;
    //     }

    //     micronet_mutate(&c_micronet);
    //     micronet_mutate(&f_micronet);

    //     double after_average_delta = get_average_delta(&target_net, &c_micronet, &f_micronet, coeff_inc);

    //     if(after_average_delta < init_average_delta) {
    //         micronet_rollback(&f_micronet);
    //         micronet_rollback(&c_micronet);
    //     }
    //     if(after_average_delta > init_average_delta) {
    //         delta_improved_counter ++;
    //     }
    //     if(i%1000 == 0) {
    //         printf("Init delta: %f, new_delta: %f, average delta: %f\n", init_average_delta, after_average_delta, delta_sum/delta_counter);
    //         fflush(stdout);
    //         if(i > 0) {
    //             micronet_save(&c_micronet, c_net_path);
    //             micronet_save(&f_micronet, f_net_path);
    //         }
    //     }
    //     if(after_average_delta > 0) {
    //         pos_new_delta_counter ++;
    //     }
    // }
    // // double current_error = get_error(&target_net, linear_dataset, sizeof_arr(linear_dataset), 1);
    // // printf("Current error: %f\n", current_error);
    // printf("Average_delta: %f, delta_counter = %d\n", delta_sum/delta_counter, delta_counter);
    // printf("Positive delta counter: %d\n", pos_delta_counter);
    // printf("Positive new delta counter: %d\n", pos_new_delta_counter);
    // printf("Improved delta counter: %d\n", delta_improved_counter);
    // printf("c_micronet coeffs:\n");
    // micronet_print_coeffs(&c_micronet);
    // printf("f_micronet coeffs:\n");
    // micronet_print_coeffs(&f_micronet);

    // // remove_folders(BCKP_DIR_PATH);
    // micronet_save(&c_micronet, c_net_path);
    // micronet_save(&f_micronet, f_net_path);
    return EXIT_SUCCESS;
}