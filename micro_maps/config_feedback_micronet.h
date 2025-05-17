#include <stdint.h>

uint32_t feedback_micronet_neurons[] = {
    // idx  num_inputs  type 	indices
	// Main ring:
	   4,	3,			1,		0, 1, 2,
	   5,	5,			1,		0, 1, 2, 4, 5,
	   6,	5,			1,		0, 1, 2, 5, 6,
	   7,	5,			1,		0, 1, 2, 6, 7,
	   8,	5,			1,		0, 1, 2, 7, 8,
	   9,	5,			1,		0, 1, 2, 8, 9,
	   10,	5,			1,		0, 1, 2, 9, 10,
	   11,	5,			1,		0, 1, 2, 10, 11,
	// Output:
	   12,	8,			1,		4, 5, 6, 7, 8, 9, 10, 11,
};

// Inputs:
//	- neuron output
//	- feedback input
//	- previous feedback output
//	- global error

network_map_t feedback_micronet_map = {
	.num_inputs = 4,
	.num_neurons = 9,
	.net_size = 13,
	.neurons = feedback_micronet_neurons,
	.num_outputs = 1,
	.output_indices = {12},
};