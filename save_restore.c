#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "network.h"
#include "save_restore.h"

static int get_int(FILE *file) {
    char temp_arr[323] = {0};
    char *temp_ptr = temp_arr;
    temp_ptr = temp_arr;
    int counter = 100;
    int result = 0;
    char temp_c = 0;
    while(counter--) {
        temp_c = (char)fgetc(file);
        if(temp_c == ' ')
            continue;
        if(temp_c < '0' || temp_c > '9')
            break;
        *temp_ptr++ = temp_c;
    }
    sscanf(temp_arr, "%d", &result);
    return result;
}

static double get_double(FILE *file) {
    char temp_arr[323] = {0};
    char *temp_ptr = temp_arr;
    temp_ptr = temp_arr;
    int counter = 100;
    double result = 0;
    while(counter--) {
        *temp_ptr = (char)fgetc(file);
        if(*temp_ptr == ',' || *temp_ptr == '\"' || *temp_ptr == ']')
            break;
        temp_ptr++;
    }
    sscanf(temp_arr, "%lf,", &result);
    return result;
}

void save_network(network_t *s, char *filename) {
    char *output = calloc(4096 * s->net_size, sizeof(char));
    char *str_ptr = output;
    str_ptr += sprintf(str_ptr, "{\n");
    str_ptr += sprintf(str_ptr, "\t\"net_size\": %u,\n", s->net_size);
    str_ptr += sprintf(str_ptr, "\t\"num_inputs\": %u,\n", s->num_inputs);
    str_ptr += sprintf(str_ptr, "\t\"num_outputs\": %u,\n", s->num_outputs);
    str_ptr += sprintf(str_ptr, "\t\"neurons\": {\n");
    for(uint32_t i=0; i<s->net_size; i++) {
        str_ptr += sprintf(str_ptr, "\t\t\"%u\": {\n", i);
        str_ptr += sprintf(str_ptr, "\t\t\t\"num_inputs\": %u,\n", s->neurons[i].num_inputs);
        str_ptr += sprintf(str_ptr, "\t\t\t\"num_coeffs\": %u,\n", s->neurons[i].num_coeffitients);
        str_ptr += sprintf(str_ptr, "\t\t\t\"inputs\": [");
        for(uint32_t j=0; j<s->neurons[i].num_inputs; j++) {
            str_ptr += sprintf(str_ptr, "{\"index\": ");
            str_ptr += sprintf(str_ptr, "%u, ", s->neurons[i].inputs[j].index);
            str_ptr += sprintf(str_ptr, "\"id\": ");
            str_ptr += sprintf(str_ptr, "%u}, ", s->neurons[i].inputs[j].id);
        }
        if(s->neurons[i].num_inputs > 0) {
            str_ptr -= 2;
            str_ptr[0] = 0;
            str_ptr[1] = 0;
        }
        str_ptr += sprintf(str_ptr, "],\n");
        str_ptr += sprintf(str_ptr, "\t\t\t\"coeffs\": [");
        for(uint32_t j=0; j<s->neurons[i].num_coeffitients; j++) {
            str_ptr += sprintf(str_ptr, "%lf, ", s->neurons[i].coeffitients[j]);
        }
        if(s->neurons[i].num_coeffitients > 0) {
            str_ptr -= 2;
            str_ptr[0] = 0;
            str_ptr[1] = 0;
        }
        str_ptr += sprintf(str_ptr, "]\n");
        str_ptr += sprintf(str_ptr, "\t\t},\n");
    }
    str_ptr -= 2;
    str_ptr += sprintf(str_ptr, "\n\t}\n");
    str_ptr += sprintf(str_ptr, "}\n");

    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        printf("Error opening file, here is the results:\n\n%s\n\n", output);
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s", output);
    fclose(file);
}

int restore_network(network_t *s, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file");
        return -1;
    }
    char temp_arr[1024] = {0};
    char *temp_ptr = temp_arr;
    uint32_t counter = 100;
    char test_c = 0;
    while(counter--) {
        test_c = (char)fgetc(file);
        if((test_c != '\t') && (test_c != '\n') && (test_c != '{') && (test_c != '}') && (test_c != ',') && (test_c != '\"')) {
            *temp_ptr++ = test_c;
        }
        if(0 == strcmp(temp_arr, "net_size: "))
        {
            memset(temp_arr, 0, sizeof(temp_arr));
            temp_ptr = temp_arr;
            s->net_size = get_int(file);
            s->neurons = (neuron_t*)malloc(s->net_size * sizeof(neuron_t));
        } else if (0 == strcmp(temp_arr, "num_inputs: "))
        {
            memset(temp_arr, 0, sizeof(temp_arr));
            temp_ptr = temp_arr;
            s->num_inputs = get_int(file);
        } else if (0 == strcmp(temp_arr, "num_outputs: "))
        {
            memset(temp_arr, 0, sizeof(temp_arr));
            temp_ptr = temp_arr;
            s->num_outputs = get_int(file);
        } else if (0 == strcmp(temp_arr, "neurons: "))
        {
            uint32_t num_neurons = 0;
            while(num_neurons < s->net_size) {
                uint32_t idx = 0;
                while(test_c != '\"')
                    test_c = (char)fgetc(file);
                memset(temp_arr, 0, sizeof(temp_arr));
                temp_ptr = temp_arr;
                test_c = (char)fgetc(file);
                while(test_c != '\"') {
                    *temp_ptr++ = test_c;
                    test_c = (char)fgetc(file);
                }
                sscanf(temp_arr, "%d", &idx);
                test_c = '0';
                while(test_c != '\"')
                    test_c = (char)fgetc(file);
                memset(temp_arr, 0, sizeof(temp_arr));
                temp_ptr = temp_arr;
                while(test_c != '}') {
                    test_c = (char)fgetc(file);
                    if((test_c != '\t') && (test_c != '\n') && (test_c != '{') && (test_c != ',') && (test_c != '\"')) {
                        *temp_ptr++ = test_c;
                    }
                    if (0 == strcmp(temp_arr, "num_inputs: ")) {
                        s->neurons[idx].num_inputs = get_int(file);
                        s->neurons[idx].inputs = (neuron_input_t*)malloc(s->neurons[idx].num_inputs * sizeof(neuron_input_t));
                        memset(temp_arr, 0, sizeof(temp_arr));
                        temp_ptr = temp_arr;
                    } else if (0 == strcmp(temp_arr, "num_coeffs: ")) {
                        s->neurons[idx].num_coeffitients = get_int(file);
                        s->neurons[idx].coeffitients = (double*)malloc(s->neurons[idx].num_coeffitients * sizeof(double));
                        memset(temp_arr, 0, sizeof(temp_arr));
                        temp_ptr = temp_arr;
                    } else if (0 == strcmp(temp_arr, "inputs: ")) {
                        uint32_t inputs_counter = 0;
                        while(test_c != '[') {
                            test_c = (char)fgetc(file);
                        }
                        test_c = (char)fgetc(file);
                        memset(temp_arr, 0, sizeof(temp_arr));
                        temp_ptr = temp_arr;
                        while(test_c != ']' && inputs_counter < s->neurons[idx].num_inputs) {
                            if((test_c != '\t') && (test_c != '\n') && (test_c != '{') && (test_c != ',') && (test_c != '\"') && (test_c != ' ')) {
                                *temp_ptr++ = test_c;
                                if (0 == strcmp(temp_arr, "index:")) {
                                    s->neurons[idx].inputs[inputs_counter].index = get_int(file);
                                    memset(temp_arr, 0, sizeof(temp_arr));
                                    temp_ptr = temp_arr;
                                } else if (0 == strcmp(temp_arr, "id:")) {
                                    s->neurons[idx].inputs[inputs_counter++].id = get_int(file);
                                    memset(temp_arr, 0, sizeof(temp_arr));
                                    temp_ptr = temp_arr;
                                }
                            }
                            test_c = (char)fgetc(file);
                        }
                        memset(temp_arr, 0, sizeof(temp_arr));
                        temp_ptr = temp_arr;
                    } else if (0 == strcmp(temp_arr, "coeffs: ")) {
                        while(test_c != '[') {
                            test_c = (char)fgetc(file);
                        }
                        uint32_t coeff_counter = 0;
                        while(coeff_counter < s->neurons[idx].num_coeffitients) {
                            s->neurons[idx].coeffitients[coeff_counter++] = get_double(file);
                        }
                        if(s->neurons[idx].num_coeffitients == 0) {
                            while(test_c != ']') {
                                test_c = (char)fgetc(file);
                            }
                        }
                        memset(temp_arr, 0, sizeof(temp_arr));
                        temp_ptr = temp_arr;
                    }
                }
                memset(temp_arr, 0, sizeof(temp_arr));
                temp_ptr = temp_arr;
                num_neurons++;
            }
        }
    }
    fclose(file);
    return 0;
}

int test_save_restore_network(char *filename) {
    network_t original_data;
    original_data.net_size = rand();
    original_data.num_inputs = rand() % 256;
    original_data.num_outputs = rand() % 256;

    save_network(&original_data, filename);

    network_t new_data;
    restore_network(&new_data, filename);

    if((original_data.net_size != new_data.net_size) &&
       (original_data.num_inputs != new_data.num_inputs) &&
       (original_data.num_outputs != new_data.num_outputs)) {
        printf("Error: The values of num_outputs do not match!\n");
        return -1;
    }
    remove(filename);
    printf("Success: The values match!\n");
    return 0;
}
