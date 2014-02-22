
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include "matrix.h"

typedef struct Network {
    Matrix* weights;
    double (*functions)(double);
    Matrix* biases_cache;
    Vector* biases;
} Network;

void feed(Network* network, double* inputs);

#endif
