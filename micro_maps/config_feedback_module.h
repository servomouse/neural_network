#include <stdint.h>

uint32_t feedback_module_neurons[] = {
    // idx  num_inputs  type 	indices
	// groups.undefined:
	   4,	5,			1,		0, 1, 2, 3, 11,
	// groups.undefined:
	   5,	5,			1,		0, 1, 2, 3, 4,
	// groups.undefined:
	   6,	5,			1,		0, 1, 2, 3, 5,
	// groups.undefined:
	   7,	5,			1,		0, 1, 2, 3, 6,
	// groups.undefined:
	   8,	5,			1,		0, 1, 2, 3, 7,
	// groups.undefined:
	   9,	5,			1,		0, 1, 2, 3, 8,
	// groups.undefined:
	   10,	5,			1,		0, 1, 2, 3, 9,
	// groups.undefined:
	   11,	5,			1,		0, 1, 2, 3, 10,
	// outputs.new_error:
	   12,	8,			1,		4, 5, 6, 7, 8, 9, 10, 11,
};

micronet_map_t feedback_module_micronet_map = {
	.num_inputs = 4,
	.num_neurons = 9,
	.net_size = 13,
	.neurons = feedback_module_neurons,
	.num_outputs = 1,
	.output_indices = {12},
};