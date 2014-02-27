
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

void feed(Network* network, Vector* input, Vector** output);

#endif
