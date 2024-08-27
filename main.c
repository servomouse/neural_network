#include "stdlib.h"
#include <stdio.h>
#include <time.h>
#include "network.h"
#include "save_restore.h"
#include "utils.h"

#define NUM_INPUTS 4
#define NUM_OUTPUTS 1

typedef struct {
    double inputs[NUM_INPUTS];
    double outputs[NUM_OUTPUTS];
} sample_t;

sample_t train_data[] = {
	{.inputs={0, 0, 0, 0}, .outputs={1}},
	{.inputs={0, 0, 0, 1}, .outputs={0}},
    {.inputs={0, 0, 1, 0}, .outputs={0}},
    {.inputs={0, 0, 1, 1}, .outputs={0}},
    {.inputs={0, 1, 0, 0}, .outputs={0}},
    {.inputs={0, 1, 0, 1}, .outputs={0}},
    {.inputs={0, 1, 1, 0}, .outputs={0}},
    {.inputs={0, 1, 1, 1}, .outputs={0}},
    {.inputs={1, 0, 0, 0}, .outputs={0}},
    {.inputs={1, 0, 0, 1}, .outputs={0}},
    {.inputs={1, 0, 1, 0}, .outputs={0}},
    {.inputs={1, 0, 1, 1}, .outputs={0}},
    {.inputs={1, 1, 0, 0}, .outputs={0}},
    {.inputs={1, 1, 0, 1}, .outputs={0}},
    {.inputs={1, 1, 1, 0}, .outputs={0}},
    {.inputs={1, 1, 1, 1}, .outputs={1}},
};

#define DATASET_SIZE sizeof(train_data)/sizeof(sample_t)

double get_error(network_t *net, double inputs[], double target_outputs[], net_config_t *net_conf) {
	double delta = 0.0;
  	double *real_output = get_output(net, inputs);
	for(int i=0; i<net_conf->num_outputs; i++) {
	    double temp = target_outputs[i] - real_output[i];
		delta += temp * temp;
	}
    return delta;
}

double average_error(network_t *net, sample_t * dataset, uint32_t dataset_size, net_config_t *net_conf) {
    double err = 0;
    for(uint32_t i=0; i<dataset_size; i++)
        err += get_error(net, dataset[i].inputs, dataset[i].outputs, net_conf);
    return err/dataset_size;
}

void test_net(network_t *net, net_config_t * net_conf) {
    for(uint32_t i=0; i<DATASET_SIZE; i++) {
        for(uint32_t j=0; j<net_conf->num_inputs; j++) {
            printf("%lf, ", train_data[i].inputs[j]);
        }
        printf("target: %lf, output: %lf\n", train_data[i].outputs[0], get_output(net, train_data[i].inputs)[0]);
    }
}

// double inputs[] = {1.0, 1.0, 1.0, 1.0};

int main(void) {
    srand(time(NULL));  // Don't need to be secure

	net_config_t net_conf = {
	  .num_inputs = NUM_INPUTS,
	  .num_neurons = 3,
	  .num_outputs = NUM_OUTPUTS
	};
    network_t *net = create_network(&net_conf);
    for(uint32_t c=0; c<10; c++) {
        for(uint32_t a=0; a<1000; a++) {
            double init_err = average_error(net, train_data, DATASET_SIZE, &net_conf);
            mutate(net);
            double new_err = average_error(net, train_data, DATASET_SIZE, &net_conf);
            if(init_err < new_err) {
                repair(net);
            }
            if(new_err < 0.000002) break;
        }
        double curr_err = average_error(net, train_data, DATASET_SIZE, &net_conf);
        double *output = get_output(net, train_data[DATASET_SIZE-1].inputs);
        print_results(net);
        printf("Current error: %lf, outputs: ", curr_err);
		for (int i=0; i<net_conf.num_outputs; i++) {
        	printf("%lf ", output[i]);
		}
    	printf("\n");
    }
    printf("Original net results:\n");
    test_net(net, &net_conf);
    save_network(net, "test_data.json");
    network_t restored;
    restore_network(&restored, "test_data.json");
    printf("Restored net results:\n");
    test_net(&restored, &net_conf);
    return 0;
}

int test_main(void) {
    srand(time(NULL));  // Don't need to be secure
    test_save_restore_network("test_data.json");
    return 0;
}
