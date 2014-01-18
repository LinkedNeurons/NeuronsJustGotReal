
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>


typedef struct vector {
    size_t size, capacity;
    float*  tab;
} vector;

vector *newVector(size_t capacity);

//those functions add at the end of the vector
void  add   (vector *v, float element);
void  delete(vector *v);
float at    (vector *v, size_t i);

void insertAt(vector *v, size_t position, float element);
void removeAt(vector *v, size_t position);

float foldl(vector *v, void (*f)(float, float), float element);
float foldr(vector *v, void (*f)(float, float), float element);

void empty(vector *v);


#endif