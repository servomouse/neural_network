#include <stdint.h>

uint32_t feedback_module_neurons[] = {
    // idx  num_inputs  type 	indices
	// groups.undefined:
	   3,	4,			1,		0, 1, 2, 10,
	// groups.undefined:
	   4,	4,			1,		0, 1, 2, 3,
	// groups.undefined:
	   5,	4,			1,		0, 1, 2, 4,
	// groups.undefined:
	   6,	4,			1,		0, 1, 2, 5,
	// groups.undefined:
	   7,	4,			1,		0, 1, 2, 6,
	// groups.undefined:
	   8,	4,			1,		0, 1, 2, 7,
	// groups.undefined:
	   9,	4,			1,		0, 1, 2, 8,
	// groups.undefined:
	   10,	4,			1,		0, 1, 2, 9,
	// outputs.feedback_out:
	   11,	8,			1,		3, 4, 5, 6, 7, 8, 9, 10,
	// outputs.stash_out:
	   12,	8,			1,		3, 4, 5, 6, 7, 8, 9, 10,
};

micronet_map_t micronet_map = {
	.num_inputs = 3,
	.num_neurons = 10,
	.net_size = 13,
	.neurons = feedback_module_neurons,
	.num_outputs = 2,
	.output_indices = {11, 12},
};