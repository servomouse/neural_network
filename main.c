#include "stdlib.h"
#include <stdio.h>
#include <time.h>
#include "network.h"
#include "utils.h"

#define NUM_INPUTS 4

typedef struct {
    double inputs[NUM_INPUTS];
    double output;
} sample_t;

sample_t train_data[] = {
    {.inputs={0, 0, 0, 0}, .output=1},
    {.inputs={0, 0, 0, 1}, .output=0},
    {.inputs={0, 0, 1, 0}, .output=0},
    {.inputs={0, 0, 1, 1}, .output=0},
    {.inputs={0, 1, 0, 0}, .output=0},
    {.inputs={0, 1, 0, 1}, .output=0},
    {.inputs={0, 1, 1, 0}, .output=0},
    {.inputs={0, 1, 1, 1}, .output=0},
    {.inputs={1, 0, 0, 0}, .output=0},
    {.inputs={1, 0, 0, 1}, .output=0},
    {.inputs={1, 0, 1, 0}, .output=0},
    {.inputs={1, 0, 1, 1}, .output=0},
    {.inputs={1, 1, 0, 0}, .output=0},
    {.inputs={1, 1, 0, 1}, .output=0},
    {.inputs={1, 1, 1, 0}, .output=0},
    {.inputs={1, 1, 1, 1}, .output=1},
};

#define DATASET_SIZE sizeof(train_data)/sizeof(sample_t)

double get_error(network_t *net, double inputs[], double target_output, uint32_t num_inputs) {
    double real_output = get_output(net, num_inputs, inputs);
    double delta = target_output - real_output;
    return delta*delta;
}

double average_error(network_t *net, sample_t * dataset, uint32_t dataset_size, uint32_t num_inputs) {
    double err = 0;
    for(uint32_t i=0; i<dataset_size; i++)
        err += get_error(net, dataset[i].inputs, dataset[i].output, num_inputs);
    return err/dataset_size;
}

void test_net(network_t *net) {
    for(uint32_t i=0; i<DATASET_SIZE; i++) {
        for(uint32_t j=0; j<NUM_INPUTS; j++) {
            printf("%lf, ", train_data[i].inputs[j]);
        }
        printf("target: %lf, output: %lf\n", train_data[i].output, get_output(net, NUM_INPUTS, train_data[i].inputs));
    }
}

// double inputs[] = {1.0, 1.0, 1.0, 1.0};

int main(void) {
    srand(time(NULL));  // Don't need to be secure

    network_t *net = create_network(4, 3, 1);
    for(uint32_t c=0; c<10; c++) {
        for(uint32_t a=0; a<1000; a++) {
            double init_err = average_error(net, train_data, DATASET_SIZE, NUM_INPUTS);
            mutate(net);
            double new_err = average_error(net, train_data, DATASET_SIZE, NUM_INPUTS);
            if(init_err < new_err) {
                repair(net);
            }
            if(new_err < 0.000002) break;
        }
        double curr_err = average_error(net, train_data, DATASET_SIZE, NUM_INPUTS);
        double output = get_output(net, NUM_INPUTS, train_data[DATASET_SIZE-1].inputs);
        print_results(net);
        printf("Current error: %lf, output: %lf\n", curr_err, output);
    }
    test_net(net);
    print_results(net);
    save_network(net, "test_data.json");
    return 0;
}

int test_main(void) {
    srand(time(NULL));  // Don't need to be secure
    test_save_restore_network("test_data.json");
    return 0;
}
