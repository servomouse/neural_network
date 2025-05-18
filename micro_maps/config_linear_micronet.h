#include <stdint.h>

uint32_t linear_micronet_neurons[] = {
    // idx  num_inputs  type 	indices
	// groups.undefined:
	   6,	6,			1,		0, 1, 2, 3, 4, 5,
	   7,	6,			1,		0, 1, 2, 3, 4, 5,
	   8,	6,			1,		0, 1, 2, 3, 4, 5,
	   9,	6,			1,		0, 1, 2, 3, 4, 5,
	   10,	6,			1,		0, 1, 2, 3, 4, 5,
	// outputs.feedback_out:
	   11,	5,			1,		6, 7, 8, 9, 10,
	// outputs.delta:
	   12,	5,			1,		6, 7, 8, 9, 10,
	// outputs.stash_out:
	   13,	5,			1,		6, 7, 8, 9, 10,
};

network_map_t linear_micronet_map = {
	.num_inputs = 6,
	.num_neurons = 8,
	.neurons = linear_micronet_neurons,
	.num_outputs = 3,
	.output_indices = {11, 12, 13},
};