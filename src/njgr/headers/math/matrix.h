#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

typedef struct Matrix {
    double* arr;
    size_t rows, cols;
} Matrix;

Matrix* matrix_create(size_t, size_t);
Matrix* matrix_init(size_t, size_t, const double*);
void matrix_destroy(Matrix*);

double matrix_get(Matrix*, size_t, size_t);
void matrix_set(Matrix*, size_t, size_t, double);

Matrix* matrix_product(Matrix*, Matrix*);
Matrix* matrix_mul(Matrix*, double);
Matrix* matrix_add(Matrix*, Matrix*);
int matrix_equals(Matrix*, Matrix*);

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

struct MatrixModule IMatrix = {
    .create  = &matrix_create,
    .init    = &matrix_init,
    .destroy = &matrix_destroy,
    .get     = &matrix_get,
    .set     = &matrix_set,
    .product = &matrix_product,
    .mul     = &matrix_mul,
    .add     = &matrix_add,
    .equals  = &matrix_equals
};

#endif // MATRIX_H_
