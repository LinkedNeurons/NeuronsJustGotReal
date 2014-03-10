#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include <math.h>
#include "math/vector.h"
#include "math/matrix.h"

typedef struct ActivationFunction {
	double (*function)(double);
	double (*derivative)(double);
} ActivationFunction;

double sigmoid(double x);
double derivative_sigmoid(double x);

struct FunctionModule {
	ActivationFunction sigmoid;
};

extern struct FunctionModule ActivationFunctions;


typedef struct Network {
	size_t depth;
	Matrix* weights;
	ActivationFunction *functions;
	Matrix* biases_cache;
	Vector* biases;
} Network;

Network* network_create(size_t depth, Matrix *weights, ActivationFunction *functions, Vector *biases);
void network_destroy(Network *network);
void network_feed0(Network* network, Vector* input, Matrix** output);
void network_feed(Network* network, Vector* input, Vector** output);
void network_gradient_descent(Network* network, Vector* input, Vector* desiredOutput);

struct NetworkModule {
	Network* (*create)(size_t, Matrix*, ActivationFunction*, Vector*);
	void (*destroy)(Network*);
	void (*feed)(Network*, Vector*, Vector**);
	void (*gradient_descent)(Network*, Vector*, Vector*);
};

extern struct NetworkModule INetwork;

#endif
