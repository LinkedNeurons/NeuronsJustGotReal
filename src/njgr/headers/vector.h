
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>


typedef struct Vector {
    size_t  size;
    double* tab;
} Vector;

Vector *vector_init	 (size_t capacity, double *tab);
Vector *vector_create(size_t size);
Vector *vector_clone (Vector* v);

//those functions add at the end of the vector
float vector_at    (Vector *v, size_t i);

float vector_foldl(Vector *v, float (*f)(float, float), float element);
float vector_foldr(Vector *v, float (*f)(float, float), float element);

#endif
