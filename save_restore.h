#pragma once

#include "stdint.h"
#include "string.h"

// neurons = {
//     0: {
//         "num_inputs": 5,
//         "num_coeffs": 32,
//         "inputs": [0, 1, 2, 3, 4],
//         "coeffs": {0.1, 0.2, 0.3 . . . ]
//     }
// }

void save_network(network_t *s, char *filename);
int restore_network(network_t *s, char *filename);
int test_save_restore_network(char *filename);
