#include <stdio.h>
#include <stdint.h>
#include "utils.h"

char *test_file_name = "test_file.bin";

int main(void) {
    uint32_t int_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double double_vals[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5};

    printf("%lld, %lld\n", sizeof(int_vals), sizeof_arr(int_vals));

    store_data(int_vals, sizeof(int_vals), test_file_name);
    uint32_t *restored_ints = restore_data(test_file_name);
    for(uint32_t i=0; i<sizeof_arr(int_vals); i++) {
        if(restored_ints[i] != int_vals[i]) {
            printf("Error: stored_value (%d) != restored_value (%d)!\n", int_vals[i], restored_ints[i]);
            return 1;
        }
    }
    free(restored_ints);
    printf("save-restore int test passed!\n");

    store_data(double_vals, sizeof(double_vals), test_file_name);
    double *restored_doubles = restore_data(test_file_name);
    for(uint32_t i=0; i<sizeof_arr(double_vals); i++) {
        if(restored_doubles[i] != double_vals[i]) {
            printf("Error: stored_value (%f) != restored_value (%f)!\n", double_vals[i], restored_doubles[i]);
            return 1;
        }
    }
    printf("save-restore double test passed!\n");
    return 0;
}