#include "micronet_save_restore.h"
#include "utils.h"
#include "neuron.h"
#include "micronet_common.h"
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

static void prepare_folders(char *path) {
    char buf[BUF_SIZE] = {0};
    snprintf(buf, BUF_SIZE, "mkdir -p %s/neurons", path);

    /* Special preprocessing for windows */
    #ifdef _WIN32   // Update path to make it work in windows
        prepare_path_for_windows(buf);
    #endif
    /* !Special preprocessing for windows */

    printf("Creating folder: %s, ", buf);
    int ret_val = system(buf);
    printf("result: %d\n", ret_val);
}

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

static void save_neurons(micro_network_t * config, char *path) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<config->num_neurons; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/neurons/neuron_%d.bin", i);
        char *n_path = concat_strings(path, buf);
        neuron_save(&config->neurons[i], n_path);
        free(n_path);
    }
}

void micronet_save(micro_network_t * config, char *path) {
    prepare_folders(path);
    char *map_path = concat_strings(path, "/map.bin");
    char *neurons_path = concat_strings(path, "/neurons.bin");
    // char *neurons_path = concat_strings(path, "/neurons");
    store_data(config->map, sizeof(micronet_map_t) + (sizeof(uint32_t) * config->num_outputs), map_path);
    store_data(config->map->neurons, get_neurons_field_size(config->map->neurons, config->num_neurons), neurons_path);
    save_neurons(config, path);
    free(map_path);
    free(neurons_path);
}

static void restore_neurons(micro_network_t * config, char *path) {
    char buf[BUF_SIZE];
    for(uint32_t i=0; i<config->num_neurons; i++) {
        clear_buffer(buf, BUF_SIZE);
        snprintf(buf, BUF_SIZE, "/neurons/neuron_%d.bin", i);
        char *n_path = concat_strings(path, buf);
        neuron_restore(&config->neurons[i], n_path);
        free(n_path);
    }
}

void micronet_restore(micro_network_t * config, char *path) {
    char *map_path = concat_strings(path, "/map.bin");
    char *neurons_path = concat_strings(path, "/neurons.bin");
    micronet_map_t *map = restore_data(map_path);
    map->neurons = restore_data(neurons_path);
    micronet_init(config, map);
    restore_neurons(config, path);
    free(map_path);
    free(neurons_path);
}