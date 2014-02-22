
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include "vector.h"
#include "math/matrix.h"

typedef struct Network {
    Matrix* weights;
    double (*functions)(double);
    Matrix* biases_cache;
    Vector* biases;
} Network;

void feed(Network* network, double* inputs, size_t num_inputs);

#endif
