#include "utils.h"
#include  <time.h>

/* generate a random double number from min to max */
double random_double(double min, double max) {
    // Make sure you have called srand(time(NULL));
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

uint32_t random_int(uint32_t min, uint32_t max) {
    uint32_t range = (max - min); 
    // uint32_t div = RAND_MAX / range;
    return min + (rand() % range);
}