#include <time.h>
#include <string.h>
#include "utils.h"
#include <math.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void sleep_ms(int ms) {
    fflush(stdout); // Need to flush the stdout buffer to not delay the prints
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void sleep_s(int s) {
    sleep_ms(s * 1000);
}

typedef struct {
    uint32_t size;  // bytes
    uint64_t hash;
    uint8_t data[0];
} stored_data_t;

int store_data(void *data, size_t size, char *filename) {
    stored_data_t data_struct = {
        .size = size,
        .hash = get_hash((uint8_t*)data, size)
    };

    FILE *file = fopen_no_matter_what(filename, "wb");
    fwrite(&data_struct, sizeof(stored_data_t), 1, file);
    fwrite((uint8_t *)data, 1, size, file);
    fclose(file);
    return EXIT_SUCCESS;
}

// Free the returned pointer after use
void *restore_data( char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    stored_data_t data_struct = {0};
    fread((uint8_t *)&data_struct, sizeof(stored_data_t), 1, file);
    uint8_t *data = calloc(data_struct.size, 1);
    size_t len_data = fread((uint8_t *)data, 1, data_struct.size, file);
    if(data_struct.hash != get_hash((uint8_t*)data, data_struct.size)) {
        printf("ERROR: File %s corrupted (hash check failed)\n", filename);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    if(len_data != data_struct.size) {
        printf("ERROR: Failed reading file %s (%lld of %d bytes were read)\n", filename, len_data, data_struct.size);
        exit(EXIT_FAILURE);
    }
    return data;
}

/* Concats two strings and returns result. Don't forget to free the result buffer after use */
char *concat_strings(const char *s1, const char *s2) {
    size_t s1_length = strlen(s1);
    size_t s2_length = strlen(s2);
    size_t total_length = s1_length + s2_length;

    char *const str_buf = malloc(total_length + 1);
    if (str_buf == NULL) {
        fprintf(stderr, "Failed to allocate memory at %s: %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    strcpy(str_buf, s1);
    strcpy(str_buf + s1_length, s2);

    return str_buf;
}

void clear_buffer(char *buffer, uint32_t buffer_size) {
    for(uint32_t i=0; i<buffer_size; i++) {
        buffer[i] = 0;
    }
}

void write_buf_to_file(const char *buffer, const char *filename) {
    FILE *file = fopen(filename, "ab");
    if (file != NULL) {
        fprintf (file, "%s", buffer);
        fclose(file);
    } else {
        printf("File write error, here are coeffs: %s", buffer);
    }
}

// Free if not NULL and then allocate the memory
void * alloc_memory(void *p, size_t num_elements, size_t sizeof_element) {
    if(p) { free(p); }
    return calloc(num_elements, sizeof_element);
}
