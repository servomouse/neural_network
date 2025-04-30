#include "network_save_restore.h"
#include "utils.h"
#include "neuron.h"
#include "network_common.h"
#include <string.h>

#define BUF_SIZE 256

/* Updates path string in place */
static void prepare_path_for_windows(char *path_string) {
    char buf2[BUF_SIZE] = {0};
    uint32_t b2_pos = 0;
    for(uint32_t i=0; i<BUF_SIZE; i++) {
        if(path_string[i] == 0) {
            break;
        } else if(path_string[i] == '/') {
            buf2[b2_pos++] = '\\';
            buf2[b2_pos++] = '\\';
        } else {
            buf2[b2_pos++] = path_string[i];
        }
    }
    clear_buffer(path_string, BUF_SIZE);
    strcpy(path_string, buf2);
}

// static void prepare_folders(char *path) {
//     char buf[BUF_SIZE] = {0};
//     snprintf(buf, BUF_SIZE, "mkdir -p %s/neurons", path);

//     /* Special preprocessing for windows */
//     #ifdef _WIN32   // Update path to make it work in windows
//         prepare_path_for_windows(buf);
//     #endif
//     /* !Special preprocessing for windows */

//     printf("Creating folder: %s, ", buf);
//     int ret_val = system(buf);
//     printf("result: %d\n", ret_val);
// }

void remove_folders(char *path) {
    char buf[BUF_SIZE] = {0};
    snprintf(buf, BUF_SIZE, "rd _s _q \"%s\"", path);

    /* Special preprocessing for windows */
    #ifdef _WIN32   // Update path to make it work in windows
        prepare_path_for_windows(buf);
    #endif
    /* !Special preprocessing for windows */
    buf[3] = '/';
    buf[6] = '/';

    printf("Removing folder: %s, ", buf);
    int ret_val = system(buf);
    printf("result: %d\n", ret_val);
}

static void save_neurons(network_t * config, char *path) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<config->num_neurons; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/neurons/neuron_%d.bin", i);
        char *n_path = concat_strings(path, buf);
        neuron_save(&config->neurons[i], n_path);
        free(n_path);
    }
}

typedef struct {
    uint32_t data_size;
    uint8_t data[0];
} neuron_data_t;

static void save_neurons_new(network_t * config, char *path) {
    uint32_t data_size = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        data_size += neuron_get_data_size(&config->neurons[i]);
    }
    uint8_t *n_data = calloc(data_size, 1);
    // n_data->data_size = data_size;
    uint32_t n_data_offset = 0;
    for(uint32_t i=0; i<config->num_neurons; i++) {
        compressed_neuron_t *c_neuron = neuron_save_new(&config->neurons[i]);
        uint8_t *c_neuron_ptr = (uint8_t*)c_neuron;
        for(uint32_t j=0; j<c_neuron->size; j++) {
            n_data[n_data_offset+j] = c_neuron_ptr[j];
        }
        n_data_offset += c_neuron->size;
        free(c_neuron);
    }

    char *n_path = concat_strings(path, "/neurons_data.bin");
    store_data(n_data, data_size, n_path);
    free(n_path);
    free(n_data);
}

void network_save(network_t * config, char *path) {
    // prepare_folders(path);
    char *map_path = concat_strings(path, "/map.bin");
    char *neurons_path = concat_strings(path, "/neurons.bin");
    // char *neurons_path = concat_strings(path, "/neurons");
    store_data(config->map, sizeof(network_map_t) + (sizeof(uint32_t) * config->num_outputs), map_path);
    store_data(config->map->neurons, get_neurons_field_size(config->map->neurons, config->num_neurons), neurons_path);
    save_neurons_new(config, path);
    free(map_path);
    free(neurons_path);
}

static void restore_neurons(network_t * config, char *path) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<config->num_neurons; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/neurons/neuron_%d.bin", i);
        char *n_path = concat_strings(path, buf);
        neuron_restore(&config->neurons[i], n_path);
        free(n_path);
    }
}

static void restore_neurons_new(network_t *net, char *path) {
    char *n_path = concat_strings(path, "/neurons_data.bin");
    uint8_t *n_data = restore_data(n_path);
    free(n_path);
    uint32_t n_data_offset = 0;
    for(uint32_t i=0; i<net->num_neurons; i++) {
        n_data_offset += neuron_restore_new(&net->neurons[i], (compressed_neuron_t *)&n_data[n_data_offset]);
        // clear_buffer(buf, BUF_SIZE);
        // snprintf(buf, BUF_SIZE, "/neurons_data.bin");
        // neuron_restore(&config->neurons[i], n_path);
    }
}

void network_restore(network_t * config, char *path, uint8_t is_micronet) {
    // printf("Network: restoring network");
    char *map_path = concat_strings(path, "/map.bin");
    char *neurons_path = concat_strings(path, "/neurons.bin");
    network_map_t *map = restore_data(map_path);
    map->neurons = restore_data(neurons_path);
    net_config_t net_config = {
        .net_map = map,
        .c_linear_micronet = NULL,
        .c_poly_micronet = NULL,
        .f_micronet = NULL
    };
    network_init(config, &net_config, is_micronet);
    restore_neurons_new(config, path);
    free(map_path);
    free(neurons_path);
}