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

double random_double(double min, double max);
uint8_t random_bit(void);
uint32_t random_int(uint32_t min, uint32_t max);
int store_data(void *data, size_t size, char *filename);
void *restore_data(char *filename);
char *concat_strings(const char *s1, const char *s2);
void gen_vector(uint32_t n, double len, double* result);double round_to_precision(double value, uint32_t precision);
uint8_t count_bits(size_t value);
void write_buf_to_file(const char *buffer, const char *filename);
void clear_buffer(char *buffer, uint32_t buffer_size);

void sleep_ms(int ms);
void sleep_s(int s);

int is_dir_exist(const char *folder);
FILE* fopen_no_matter_what(const char *file_name, const char *access_mode);
int remove_directory(const char *dir);
