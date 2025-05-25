#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __unix__
    #define DLL_PREFIX 
#elif defined(_WIN32) || defined(WIN32)
    #define DLL_PREFIX __declspec(dllexport)
#endif

#define sizeof_arr(_x) sizeof(_x)/sizeof(_x[0])

void sleep_ms(int ms);
void sleep_s(int s);
int store_data(void *data, size_t size, char *filename);
void *restore_data(char *filename);
char *concat_strings(const char *s1, const char *s2);
void write_buf_to_file(const char *buffer, const char *filename);
void clear_buffer(char *buffer, uint32_t buffer_size);
void * alloc_memory(void *p, size_t num_elements, size_t sizeof_element);

// mymath.c:
double random_double(double min, double max);
uint8_t random_bit(void);
uint32_t random_int(uint32_t min, uint32_t max);
void gen_vector(uint32_t n, double len, double* result);
void gen_complex_vector(uint32_t n, double len, void* result);
double round_to_precision(double value, uint32_t precision);
uint8_t count_bits(size_t value);
uint64_t get_hash(uint8_t *data, size_t size);

// work_with_folders.c:
// Returns 1 if the folder exists, 0 if it doesn't
int is_dir_exist(const char *folder);
// Opens the given file, creates any missing paths of the path if needed
FILE* fopen_no_matter_what(const char *filename, const char *access_mode);
int remove_directory(const char *dir);

