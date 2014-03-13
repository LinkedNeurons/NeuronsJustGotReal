#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>
#include "math/vector.h"

typedef struct Matrix {
	double* arr;
	size_t rows, cols;
} Matrix;

Matrix* matrix_create(size_t, size_t);
Matrix* matrix_init(size_t, size_t, const double*);
void matrix_destroy(Matrix*);

double matrix_get(Matrix*, size_t, size_t);
void matrix_set(Matrix*, size_t, size_t, double);

void matrix_product(Matrix*, Matrix*, Matrix**);
void matrix_mul(Matrix*, double, Matrix**);
void matrix_add(Matrix*, Matrix*, Matrix**);
void matrix_substract(Matrix*, Matrix*, Matrix**);
void matrix_vectorize(Matrix*, Vector**);
int matrix_equals(Matrix*, Matrix*);

void matrix_apply(Matrix*, double (*f)(double), Matrix**);

void matrix_repmat(Vector* v, size_t rows, size_t cols, Matrix**);

/* Module */
struct MatrixModule {
	Matrix* (*create)(size_t, size_t);
	Matrix* (*init)(size_t, size_t, const double*);
	void (*destroy)(Matrix*);
	double (*get)(Matrix*, size_t, size_t);
	void (*set)(Matrix*, size_t, size_t, double);
	void (*product)(Matrix*, Matrix*, Matrix**);
	void (*mul)(Matrix*, double, Matrix**);
	void (*add)(Matrix*, Matrix*, Matrix**);
	void (*substract)(Matrix*, Matrix*, Matrix**);
	void (*vectorize)(Matrix*, Vector**);
	void (*apply)(Matrix*, double (*)(double), Matrix**);
	int (*equals)(Matrix*, Matrix*);
	void (*repmat)(Vector*, size_t, size_t, Matrix**);
};

extern struct MatrixModule IMatrix;

#endif // MATRIX_H_
