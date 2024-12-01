#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __unix__
    #define DLL_PREFIX 
#elif defined(_WIN32) || defined(WIN32)
    #define DLL_PREFIX __declspec(dllexport)
#endif

double random_double(double min, double max);
uint32_t random_int(uint32_t min, uint32_t max);
int store_data(void *data, size_t size, char *filename);
int restore_data(void *data, size_t size, char *filename);
char *concat_strings(const char *s1, const char *s2);
