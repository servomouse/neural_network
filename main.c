#include "stdlib.h"
#include <stdio.h>
#include <time.h>
#include "network.h"

int main(void) {
    srand(time(NULL));  // Don't need to be secure

    network_t *a = create_network(8, 2, 1);
    printf("Hello world!\n");
}