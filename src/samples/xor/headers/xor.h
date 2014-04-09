#ifndef XOR_H_
#define XOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void xor_init();
void xor_run();

struct neuron {
	double inputs[2];
	double weights[2];
	double error;
	double bias;
};

double sigmoid(double x);
double derivative(double x);
double randd();
struct neuron* xor_neuron_create();
void adjust(struct neuron* n);
double get_output(struct neuron* n);

#endif // XOR_H_
