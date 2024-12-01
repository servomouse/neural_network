#include <stdlib.h>
#include "neuron_kapellmeister.h"
#include "utils.h"

typedef struct {
    uint32_t num_inputs;
    uint32_t num_neurons;
    uint32_t net_size;
    double *arr[2];
    uint32_t input_arr;
    uint32_t output_arr;
    get_output_func_t *output_func;
} network_t;

network_t net;

DLL_PREFIX
void init(uint32_t num_inputs, uint32_t num_neurons) {
    net.num_inputs = num_inputs;
    net.num_neurons = num_neurons;
    net.net_size = num_inputs + num_neurons;
    net.arr[0] = calloc(net.net_size, sizeof(double));
    net.arr[1] = calloc(net.net_size, sizeof(double));
    net.input_arr = 0;
    net.output_arr = 1;
    net.output_func = calloc(num_neurons, sizeof(get_output_func_t));
}

DLL_PREFIX
void set_output_function(uint32_t idx, get_output_func_t func) {
    net.output_func[idx] = func;
}

DLL_PREFIX
void set_value(uint32_t idx, double value) {
    net.arr[net.input_arr][idx] = value;
}

DLL_PREFIX
double get_value(uint32_t idx) {
    printf("Getting value of network[%d]: %lf\n", idx, net.arr[net.input_arr][idx]);
    return net.arr[net.input_arr][idx];
}

DLL_PREFIX
void tick_network(void) {
    for(int i=0; i<net.num_inputs; i++) {
        net.arr[net.output_arr][i] = net.arr[net.input_arr][i];
        printf("network[%d] = %lf\n", i, net.arr[net.output_arr][i]);
    }
    for(int i=net.num_inputs; i<net.net_size; i++) {
        net.arr[net.output_arr][i] = net.output_func[i](net.arr[net.input_arr]);
        printf("network[%d] = %lf\n", i, net.arr[net.output_arr][i]);
    }
}

DLL_PREFIX
void swap_arrays(void) {
    net.input_arr ^= net.output_arr;
    net.output_arr ^= net.input_arr;
    net.input_arr ^= net.output_arr;
}

DLL_PREFIX
void save_state(char *filename) {
    char *arr0_fname = concat_strings("arr0_", filename);
    char *arr1_fname = concat_strings("arr1_", filename);
    store_data(&net, sizeof(net), filename);
    store_data(net.arr[0], net.net_size * sizeof(double), arr0_fname);
    store_data(net.arr[1], net.net_size * sizeof(double), arr1_fname);
    free(arr0_fname);
    free(arr1_fname);
}

DLL_PREFIX
void restore_state(char *filename) {
    if(!net.output_func) {
        printf("Call init() before calling restore_state()!");
        exit(1);
    }
    char *arr0_fname = concat_strings("arr0_", filename);
    char *arr1_fname = concat_strings("arr1_", filename);
    restore_data(&net, sizeof(net), filename);
    // net.arr[0] = malloc(net.net_size * sizeof(double));
    // net.arr[1] = malloc(net.net_size * sizeof(double));
    restore_data(net.arr[0], net.net_size * sizeof(double), arr0_fname);
    restore_data(net.arr[1], net.net_size * sizeof(double), arr1_fname);
    free(arr0_fname);
    free(arr1_fname);
}
