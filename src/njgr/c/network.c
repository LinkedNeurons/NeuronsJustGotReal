#include <stdlib.h>
#include <stdio.h>

#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"

struct NetworkModule INetwork = {
	.create = &network_create,
	.feed   = &network_feed
};

void *ignore(void * v) {
    return v;
}

Network* network_create(int depth, Matrix *weights, ActivationFunction *functions, Vector *biases) {
    ignore(&depth);
    ignore(weights);
    ignore(functions);
    ignore(biases);
    return NULL;
}

void network_feed(Network* network, Vector* input, Vector** output) {
	Matrix* column                 = IMatrix.init(input->size, 1, input->tab);
	Matrix* weights                = network->weights;
	ActivationFunction *activation = network->functions;
	Matrix* biases_cache           = network->biases_cache;
	Matrix* out = NULL;

	for (size_t i = 0; i < network->depth; ++i) {
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

Vector* network_to_vect(Network* network) 
{
    Matrix* weights                = network->weights;
    double* tab = malloc(network->size * sizeof(double));

    size_t curr = 0;

    for (size_t i = 0; i < network->depth; ++i) {
        for (size_t j = 0; j < weights->rows * weights->cols; ++j) {
                tab[curr] = weights->arr[j];
                ++curr;
        }
        ++weights;
    }

    Vector* v = IVector.init(network->size, tab);

    return v;
}
