
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

typedef vector *vector_t;

struct vector_t {
    size_t size, capacity;
    void*  tab;
};

vector newVector(size_t capacity);

void add    (vector v, void *element);
void remove (vector v, void *element);
void at     (vector v, int i);

void insert(vector v, int i, void *element);

void foldl(vector v, void (*f)(void *));
void foldr(vector v, void (*f)(void *));

void empty(vector v);


#endif