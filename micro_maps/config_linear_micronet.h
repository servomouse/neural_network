#include <stdint.h>
#include "micronet.h"

uint32_t linear_micronet_neurons[] = {
    // idx  num_inputs  type 	indices
	// groups.undefined:
	   5,	5,			1,		0, 1, 2, 3, 4,
	   6,	5,			1,		0, 1, 2, 3, 4,
	   7,	5,			1,		0, 1, 2, 3, 4,
	   8,	5,			1,		0, 1, 2, 3, 4,
	   9,	5,			1,		0, 1, 2, 3, 4,
	// outputs.feedback_out:
	   10,	5,			1,		5, 6, 7, 8, 9,
	// outputs.delta:
	   11,	5,			1,		5, 6, 7, 8, 9,
	// outputs.stash_out:
	   12,	5,			1,		5, 6, 7, 8, 9,
};

micronet_map_t linear_micronet_map = {
	.num_inputs = 5,
	.num_neurons = 8,
	.net_size = 13,
	.neurons = linear_micronet_neurons,
	.num_outputs = 3,
	.output_indices = {10, 11, 12},
};