#include <stdlib.h>
#include <stdio.h>

#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"

void feed(Network* network, Vector* input, Vector** output) {
	Matrix* column                 = IMatrix.init(input->size, 1, input->tab);
	Matrix* weights                = network->weights;
	ActivationFunction *activation = network->functions;
	Matrix* biases_cache           = network->biases_cache;
	Matrix* out = NULL;

	for (int i = 0; i < network->depth; ++i) {
		IMatrix.product(weights, column, &out);
		IMatrix.add(out, biases_cache, &out);
		IMatrix.apply(out, activation->function, &out);
		free(column);
		column = out;
		++weights;
		++activation;
		++biases_cache;
		out = NULL;
	}

	IMatrix.vectorize(out, output);
}
