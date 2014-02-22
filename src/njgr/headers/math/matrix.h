#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

typedef struct Matrix {
    double* arr;
    size_t rows, cols;
} Matrix;

typedef struct MatrixExpressionFactory {
	
} MatrixExpressionFactory;

Matrix* matrix_create(size_t, size_t);
Matrix* matrix_init(size_t, size_t, const double*);
void matrix_destroy(Matrix*);

double matrix_get(Matrix*, size_t, size_t);
void matrix_set(Matrix*, size_t, size_t, double);

Matrix* matrix_product(Matrix*, Matrix*);
Matrix* matrix_mul(Matrix*, double);
Matrix* matrix_add(Matrix*, Matrix*);
int matrix_equals(Matrix*, Matrix*);

Matrix* matrix_apply(Matrix*, double (*f)(double)); 

typedef void* (*binary)(void*,void*);
typedef void* (*unary)(void*);
MatrixExpressionFactory* mef_chain_binary(binary op, void* a, void* b);
MatrixExpressionFactory* mef_chain_unary(unary op, void* a);

struct MatrixModule {
    Matrix* (*create)(size_t, size_t);
    Matrix* (*init)(size_t, size_t, const double*);
    void (*destroy)(Matrix*);
    double (*get)(Matrix*, size_t, size_t);
    void (*set)(Matrix*, size_t, size_t, double);
    Matrix* (*product)(Matrix*, Matrix*);
    Matrix* (*mul)(Matrix*, double);
    Matrix* (*add)(Matrix*, Matrix*);
    int (*equals)(Matrix*, Matrix*);
};

extern struct MatrixModule IMatrix;

#endif // MATRIX_H_
