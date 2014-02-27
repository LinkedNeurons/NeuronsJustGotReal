#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

typedef struct Vector {
    size_t  size;
    double* tab;
} Vector;

Vector *vector_init  (size_t capacity, double *tab);
Vector *vector_create(size_t size);
Vector *vector_clone (Vector* v);

double vector_at    (Vector *v, size_t i);

double vector_foldl(Vector *v, double (*f)(double, double), double element);
double vector_foldr(Vector *v, double (*f)(double, double), double element);

struct VectorModule {
	Vector* (*init)(size_t, double*);
	Vector* (*create)(size_t);
	Vector* (*clone)(Vector*);
	double (*at)(Vector*, size_t);
	double (*foldl)(Vector*, double (*)(double, double), double);
	double (*foldr)(Vector*, double (*)(double, double), double);
};

extern struct VectorModule IVector;

#endif
