
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include "math/vector.h"
#include "math/matrix.h"

typedef struct ActivationFunction {
	double (*function)(double);
	double (*derivative)(double);
} ActivationFunction;

typedef struct Network {
	int depth;
    Matrix* weights;
    ActivationFunction *functions;
    Matrix* biases_cache;
    Vector* biases;
} Network;

Network* network_create(int depth, Matrix *weights, ActivationFunction *functions, Vector *biases);
void network_feed(Network* network, Vector* input, Vector** output);

struct NetworkModule {
	Network* (*create)(int depth, Matrix*, ActivationFunction*, Vector*);
	void (*feed)(Network*, Vector*, Vector**);
};

extern struct NetworkModule INetwork;

#endif
