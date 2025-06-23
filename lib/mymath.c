#include <math.h>
#include "utils.h"

/* generate a random double number from min to max */
double random_double(double min, double max) {
    // Make sure you have called srand(time(NULL));
    double range = (max - min); 
    double div = RAND_MAX / range;
    return round_to_precision(min + (rand() / div), 5);
}

uint8_t random_bit(void) {
    return rand() & 1;
}

uint8_t count_bits(size_t value) {
    uint8_t counter = 0;
    for(uint8_t i=0; i<(8*sizeof(size_t)); i++) {
        if(value & ((size_t)1<<i)) {
            counter++;
        }
    }
    return counter;
}

// Returns a random integer, min values is min, max value is max-1
uint32_t random_int(uint32_t min, uint32_t max) {
    uint32_t range = (max - min); 
    // uint32_t div = RAND_MAX / range;
    return min + (rand() % range);
}

// Generates a random vector of given length in an n-dimensional space
void gen_vector(uint32_t n, double len, double* result) {
    // Generate random components
    double sum_of_squares = 0.0;
    for (uint32_t i = 0; i < n; ++i) {
        result[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Random number between -1 and 1
        sum_of_squares += result[i] * result[i];
    }

    // Normalize the vector to have unit length
    double norm = sqrt(sum_of_squares);
    for (uint32_t i = 0; i < n; ++i) {
        result[i] /= norm;
    }

    // Scale the vector to the desired length
    for (uint32_t i = 0; i < n; ++i) {
        result[i] *= len;
    }

    // Cut off the trailing digits
    for (uint32_t i = 0; i < n; ++i) {
        result[i] = round_to_precision(result[i], 5);
    }
}

static double complex_vector_length(uint32_t n, void* vector_2d) {
    typedef struct {
        double val[2];
    } complex_vector_t;
    complex_vector_t *vector = (complex_vector_t*)vector_2d;
    double sum_of_squares = 0.0;
    for (uint32_t i=0; i<n; i++) {
        sum_of_squares += vector[i].val[0] * vector[i].val[0] + vector[i].val[1] * vector[i].val[1];
    }
    double norm = sqrt(sum_of_squares);
    return norm;
}

// Generates a random 2d vector (a complex vector) of given length in an n-dimensional space
void gen_complex_vector(uint32_t n, double len, void* result) {
    typedef struct {
        double val[2];
    } complex_vector_t;
    complex_vector_t *vector = (complex_vector_t*)result;
    // Generate random components
    for(uint32_t i=0; i<n; i++) {
            vector[i].val[0] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Random number between -1 and 1
            vector[i].val[1] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Random number between -1 and 1
    }

    // Get length
    double norm = complex_vector_length(n, vector);

    // Normalize the vector to have unit length
    for (uint32_t i = 0; i < n; ++i) {
        vector[i].val[0] /= norm;
        vector[i].val[1] /= norm;
    }

    // Scale the vector to the desired length
    for (uint32_t i = 0; i < n; ++i) {
        vector[i].val[0] *= len;
        vector[i].val[1] *= len;
    }

    // Cut off the trailing digits
    for (uint32_t i = 0; i < n; ++i) {
        vector[i].val[0] = round_to_precision(vector[i].val[0], 5);
        vector[i].val[1] = round_to_precision(vector[i].val[1], 5);
    }
}

// Precision is how many digits after the decimal point
double round_to_precision(double value, uint32_t precision) {
    double factor = pow(10.0, precision);
    return round(value*factor)/factor;
}

uint64_t get_hash(uint8_t *data, size_t size) {
    uint64_t hash = 5381;
    for(size_t i=0; i<size; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

int are_equal(double val1, double val2, int precision) {
    double factor = pow(10.0, precision);
    
    long long scaled_val1 = (long long)round(val1 * factor);
    long long scaled_val2 = (long long)round(val2 * factor);
    
    return scaled_val1 == scaled_val2;
}
