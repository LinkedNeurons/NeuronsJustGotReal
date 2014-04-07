#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "math/vector.h"

struct VectorModule IVector = {
	.init          = &vector_init,
	.initf         = &vector_initf,
	.create        = &vector_create,
	.destroy       = &vector_destroy,
	.destroy_array = &vector_destroy_array,
	.clone         = &vector_clone,
	.at            = &vector_at,
	.foldr         = &vector_foldr,
	.foldl         = &vector_foldl,
	.equals        = &vector_equals
};

void vector_init(size_t size, double *tab, Vector** out) {
	if (!*out) {
		*out = malloc(sizeof(Vector));
	}
	(*out)->tab    = malloc(size * sizeof(double));
	(*out)->size   = size;

	for(size_t i = 0; i < size; ++i) { (*out)->tab[i] = tab[i]; }
}

void vector_initf(size_t size, double (*f)(size_t), Vector** out) {
	if (!*out) {
		*out = malloc(sizeof(Vector));
	}
	(*out)->tab    = malloc(size * sizeof(double));
	(*out)->size   = size;

	for(size_t i = 0; i < size; ++i) { (*out)->tab[i] = f(i); }
}

Vector *vector_create(size_t size) {
	Vector *v = malloc(sizeof(Vector));
	v->tab    = malloc(size * sizeof(double));
	v->size   = size;

	for(size_t i = 0; i < size; ++i) { v->tab[i] = 0; }

	return v;
}

void vector_destroy(Vector *v) {
	free(v->tab);
	free(v);
}

void vector_destroy_array(Vector *array, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		free(array[i].tab);
	}
	free(array);
}

Vector *vector_clone(Vector *v) {
	Vector *clone = NULL;
	vector_init(v->size, v->tab, &clone);
	return clone;
}

double vector_at(Vector *v, size_t i) {
	return v->tab[i];
}


double vector_foldl(Vector *v, double (*f)(double, double), double element) {
	if(v->size == 0) { return element; }
	
	for(size_t i = 0; i < v->size; i++) {
		element = (*f)(element, v->tab[i]);
	}

	return element;
}

double vector_foldr(Vector *v, double (*f)(double, double), double element) {
	if(v->size == 0) { return element; }
	
	for(size_t i = v->size - 1; i != 0; i--) {
		element = (*f)(v->tab[v->size - i], element);
	}

	return element;
}

int vector_equals(Vector *v1, Vector *v2) {
	int equals = v1->size == v2->size;
	for (size_t i = 0; equals && i < v1->size; ++i) {
		equals = equals && vector_at(v1, i) == vector_at(v2, i);
	}
	return equals;
}
