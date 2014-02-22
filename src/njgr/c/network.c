#include <stdlib.h>
#include <stdio.h>

#include "network.h"

void feed(Network* network, double* input, size_t num_inputs) {
	Matrix* column  = IMatrix.init(num_inputs, 1, input);
	Matrix* weights = network->weights;

	Matrix* z = IMatrix.product(weights, column);
}
