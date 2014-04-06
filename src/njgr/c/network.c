#include <stdlib.h>
#include <stdio.h>

#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "util/swissknife.h"

struct NetworkModule INetwork = {
	.create  = &network_create,
	.destroy = &network_destroy,
	.feed    = &network_feed
};

Network* network_create(size_t depth, Matrix *weights, ActivationFunction *functions, Vector *biases) {
	Matrix* biases_cache  = malloc(sizeof(Matrix) * (depth - 1));
	for (size_t i = 0; i < depth - 1; ++i) {
		Matrix* rep = biases_cache + i;
		IMatrix.repmat(&biases[i], 1, weights[i].cols, &rep);
	}

	Network *network = NULL;
	new_struct(network, Network) {
		.depth        = depth,
		.weights      = weights,
		.functions    = functions,
		.biases       = biases,
		.biases_cache = biases_cache
	};

	return network;
}

void network_destroy(Network *network) {
	IMatrix.destroy_array(network->weights, network->depth - 1);
	IMatrix.destroy_array(network->biases_cache, network->depth - 1);
	IVector.destroy_array(network->biases, network->depth - 1);
	free(network->functions);
	free(network);
}

void network_feed(Network* network, Vector* input, Vector** output) {
	Matrix* column = NULL;
	IMatrix.init(input->size, 1, input->tab, &column);
	Matrix* weights                = network->weights;
	ActivationFunction *activation = network->functions;
	Matrix* biases_cache           = network->biases_cache;
	Matrix* out = NULL;

	for (size_t i = 1; i < network->depth; ++i) {
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

	IMatrix.vectorize(column, output);
}

// Activation functions

struct FunctionModule ActivationFunctions = {
	.sigmoid = { .function = &sigmoid, .derivative = &derivative_sigmoid }
};


double sigmoid(double x) {
	return 1. / (1 + exp(-x));
}

double derivative_sigmoid(double x) {
	double sig = sigmoid(x);
	return sig * (1 - sig);
}
