#include <stdlib.h>
#include <stdio.h>

#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "util/swissknife.h"

struct NetworkModule INetwork = {
	.create  = &network_create,
	.destroy = &network_destroy,
	.feed    = &network_feed,
	.gradient_descent = &network_gradient_descent
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

void network_feed0(Network* network, Vector* input, Matrix** output) {
	if (!*output) {
		*output = malloc(sizeof(Matrix) * (network->depth - 1));
	}

	Matrix* column = NULL;
	IMatrix.init(input->size, 1, input->tab, &column);
	Matrix* weights                = network->weights;
	ActivationFunction *activation = network->functions;
	Matrix* biases_cache           = network->biases_cache;
	Matrix* out = NULL;
	Matrix* m = *output;

	for (size_t i = 1; i < network->depth; ++i) {
		IMatrix.product(weights, column, &out);
		IMatrix.add    (out, biases_cache, &out);
		IMatrix.apply  (out, activation->function, &out);
		free(column);
		column = out;
		*m = *out;
		++weights;
		++activation;
		++biases_cache;
		++m;
		out = NULL;
	}
}

void network_feed(Network* network, Vector* input, Vector** output) {
	Matrix *out = NULL;
	network_feed0(network, input, &out);
	IMatrix.vectorize(out + network->depth - 2, output);
	IMatrix.destroy_array(out, network->depth - 1);
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

void network_gradient_descent(Network* network, Vector* in, Vector* desiredOutput) {
	Matrix* outputs = NULL;
	network_feed0(network, in, &outputs);

	double  epsilon				   = 1;
	ActivationFunction *activation = network->functions + network->depth - 2;
	Matrix* weight                 = network->weights + network->depth - 2;
	Matrix* output   			   = outputs + (network->depth - 2);
	Matrix* input   			   = outputs + (network->depth - 3);
	Matrix* delta				   = NULL;

	Matrix *desired = NULL;
	IMatrix.init(desiredOutput->size, 1, desiredOutput->tab, &desired);
	IMatrix.substract(output, desired, &delta);
	for(size_t i = 1; i < network->depth; i++) {

		IMatrix.apply(output, activation->derivative, &output);
		IMatrix.mul(delta, epsilon, &delta);

		Matrix *tmp = NULL, *tmp2 = NULL;
		IMatrix.product(output, input, &tmp);
		IMatrix.product(tmp, delta, &tmp2);
		IMatrix.add(weight, tmp2, &weight);

		IMatrix.destroy(tmp);
		IMatrix.destroy(tmp2);

		--output; --input;
		--weight; --activation;
	}
}
