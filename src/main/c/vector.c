#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../headers/vector.h"

vector *newVector(size_t capacity) {
    vector *v = malloc(sizeof(struct vector));
    v->tab    = malloc(capacity * sizeof(float));
    v->size = 0; v->capacity = capacity;

    return v;
}

void resize(vector *v) {
    if(v->size == v->capacity) {

        int capacity = v->capacity;
        v->capacity  = (int)ceil(1.5 * v->capacity);

        float *tab = malloc(v->capacity * sizeof(float));
        for(int i = 0; i < capacity; ++i) {
            tab[i] = v->tab[i];
        }

        free(v->tab);
        v->tab = tab;
    }
}

void add(vector *v, float element) {
    v->size++;
    resize(v);

    v->tab[v->size] = element;
}

void delete(vector *v) {
    v->size--;
    resize(v);
}

float at(vector *v, unsigned int i) {
    return v->tab[i];
}

void insertAt(vector *v, unsigned int position, float element) {
    v->size++;
    resize(v);

    for(unsigned int i = v->size; i >= position; ++i) {
        v->tab[i] = v->tab[i - 1];
    }

    v->tab[position] = element;
}

void removeAt(vector *v, unsigned int position) {
    v->size--;
    resize(v);

    for(unsigned int i = position; i < v->size; ++i) {
        v->tab[i] = v->tab[i + 1];
    }
}

float foldl(vector *v, void (*f)(float, float), float element) {
    if(v->size == 0) { return element; }
    
    for(size_t i = 0; i < v->size; i++) {
        element = f(element, v->tab[i]);
    }

    return element;
}

float foldr(vector *v, void (*f)(float, float), float element) {
    if(v->size == 0) { return element; }
    
    for(size_t i = v->size - 1; i >= 0; i++) {
        element = f(v->tab[v->size - i], element);
    }

    return element;
}