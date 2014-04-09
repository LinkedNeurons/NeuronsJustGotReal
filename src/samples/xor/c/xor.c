#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "xor.h"

double sigmoid(double x) {
	return 1. / (1. + exp(-x));
}

double derivative(double x) {
	return x * (1 - x) + 0.01;
}

double randd() {
	return (double) rand() / (double) RAND_MAX;
}

struct neuron* xor_neuron_create() {
	struct neuron* n = malloc(sizeof(struct neuron));
	n->inputs[0] = 0;
	n->inputs[1] = 0;
	n->weights[0] = randd();
	n->weights[1] = randd();
	n->bias = randd();
	return n;
}

void adjust(struct neuron* n) {
	n->weights[0] += n->error * n->inputs[0];
	n->weights[1] += n->error * n->inputs[1];
	n->bias += n->error;
}

double get_output(struct neuron* n) {
	return sigmoid(n->weights[0] * n->inputs[0] + n->weights[1] * n->inputs[1] + n->bias);
}

void xor_init() {
	srand((unsigned) time(NULL));
}

void xor_run() {
	double inputs[] = { 0, 0, 0, 1, 1, 0, 1, 1 };
	double expected[] = {0, 1, 1, 0};

	struct neuron* hidden1 = xor_neuron_create();
	struct neuron* hidden2 = xor_neuron_create();
	struct neuron* output = xor_neuron_create();

	int epoch = 0;
	train: {
		++epoch;
		
		double global_error = 0;
		for (int i = 0; i < 4; ++i) {
			hidden1->inputs[0] = inputs[i << 1];
			hidden1->inputs[1] = inputs[(i << 1) + 1];
			hidden2->inputs[0] = inputs[i << 1];
			hidden2->inputs[1] = inputs[(i << 1) + 1];
			output ->inputs[0] = get_output(hidden1);
			output ->inputs[1] = get_output(hidden2);

			double out = get_output(output);
			output->error = derivative(out) * (expected[i] - out);
			adjust(output);

			hidden1->error = derivative(get_output(hidden1)) * output->error * output->weights[0];
			adjust(hidden1);

			hidden2->error = derivative(get_output(hidden2)) * output->error * output->weights[1];
			adjust(hidden2);

			global_error += fabs(expected[i] - out);
		}
		global_error /= 4.;

		if (epoch % 1000 == 0) printf("[epoch %d]: global_error: %.10f\n", epoch, global_error);
		if (global_error > 0.1) {
			if (epoch > 700) { goto reload; }
			goto train;
		}
	}
	printf("converged in %d epochs\n", epoch);
	for (int i = 0; i < 4; ++i) {
		hidden1->inputs[0] = inputs[i << 1];
		hidden1->inputs[1] = inputs[(i << 1) + 1];
		hidden2->inputs[0] = inputs[i << 1];
		hidden2->inputs[1] = inputs[(i << 1) + 1];
		output ->inputs[0] = get_output(hidden1);
		output ->inputs[1] = get_output(hidden2);

		double out = get_output(output);
		printf("\t%f xor %f = %f\n", hidden1->inputs[0], hidden1->inputs[1], out);
	}

	return;
	reload: xor_run();
}
