#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vector.h"

Vector *vector_init(size_t size, double *tab) {
	Vector *v = malloc(sizeof(Vector));
	v->tab    = malloc(size * sizeof(double));
	v->size   = size;

	for(size_t i = 0; i < size; ++i) { v->tab[i] = tab[i]; }

	return v;
}

Vector *vector_create(size_t size) {
	Vector *v = malloc(sizeof(Vector));
	v->tab    = malloc(size * sizeof(double));
	v->size   = size;

	for(size_t i = 0; i < size; ++i) { v->tab[i] = 0; }

	return v;
}

Vector *vector_clone(Vector *v) {
	Vector *clone = malloc(sizeof(Vector));
	clone->tab    = malloc(v->size * sizeof(double));
	clone->size   = v->size;

	for(size_t i = 0; i < v->size; ++i) { clone->tab[i] = v->tab[i]; }

	return v;
}

float vector_at(Vector *v, size_t i) {
    return v->tab[i];
}


float vector_foldl(Vector *v, float (*f)(float, float), float element) {
    if(v->size == 0) { return element; }
    
    for(size_t i = 0; i < v->size; i++) {
        element = (*f)(element, v->tab[i]);
    }

    return element;
}

float vector_foldr(Vector *v, float (*f)(float, float), float element) {
    if(v->size == 0) { return element; }
    
    for(size_t i = v->size - 1; i != 0; i--) {
        element = (*f)(v->tab[v->size - i], element);
    }

    return element;
}
