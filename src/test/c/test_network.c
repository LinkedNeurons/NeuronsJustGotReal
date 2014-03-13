#include <stdio.h>

#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"

void network_test_feed() {
	Matrix* weights = malloc(sizeof(Matrix) * 2);
	Matrix* w1 = IMatrix.init(2, 2, (double[]) {0.5, 0.5, 0.5, 0.5});
	Matrix* w2 = IMatrix.init(1, 2, (double[]) {0.5, 0.5});
	weights[0] = *w1;
	weights[1] = *w2;
	free(w1);
	free(w2);
	Vector* biases = malloc(sizeof(Vector) * 2);
	Vector* b1 = IVector.init(2, (double[]) {1, 1});
	Vector* b2 = IVector.init(1, (double[]) {1});
	biases[0] = *b1;
	biases[1] = *b2;
	free(b1);
	free(b2);
	ActivationFunction *functions = malloc(sizeof(ActivationFunction) * 2);
	functions[0] = ActivationFunctions.sigmoid;
	functions[1] = ActivationFunctions.sigmoid;
	Network* network = INetwork.create(3, weights, functions, biases);

	Vector* inputs = IVector.init(2, (double[]) { 1, 1 });
	Vector* output = NULL;
	INetwork.feed(network, inputs, &output);
	
	printf("%f\n", IVector.at(output, 0));
}
