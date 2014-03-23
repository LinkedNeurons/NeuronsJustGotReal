#include <stdio.h>
#include <stdlib.h>
#include "genetic.h"
#include "math/vector.h"

void order_pop(Population *pop, int *scores)
{
    
    for (size_t i = 1; i < pop->size; ++i) {
        
        int j = i;
        for (; j > 0 && scores[j-1] > scores[i]; --j) {
            scores[j] = scores[j-1];
            pop->tab[j] = pop->tab[j-1];
        }
        
        scores[j] = scores[i];
        pop->tab[j] = pop->tab[i];
    }
}

Vector* mix_netw(Vector* n1, Vector* n2)
{
    double *netw = malloc(n1->size * sizeof(double));
    
    for (size_t i = 0; i < n1->size; i++)
        netw[i] = i / 10 % 2 ? n1->tab[i] : n2->tab[i];
    
    Vector* v = IVector.init(n1->size, netw);

    return v;
}

Vector* gen_netw(size_t size_netw)
{
    double *netw = malloc(size_netw * sizeof(double));
    
    for (size_t i = 0; i<size_netw; i++)
        netw[i] = rand() - 0.5;
    
    Vector* v = IVector.init(size_netw, netw);
    
    return v;
}

Population* create_pop(Vector** netws, size_t size)
{
    Population* pop = malloc(sizeof(struct s_popu));
    pop->tab = netws;
    pop->size = size;

    return pop;
}

Population* get_new_pop(Population *pop, int *scores, size_t nb_const, size_t nb_new)
{
    Vector** new_pop =  malloc(pop->size * sizeof(Vector*));

    for (size_t i = 0; i < pop->size; ++i)
        new_pop[i] = IVector.create(pop->tab[0]->size);

    order_pop(pop, scores);
    
    size_t i = 0;
    for (; i < nb_const; ++i)
        new_pop[i] = pop->tab[i];
    
    for (; i<pop->size - nb_new; ++i)
        new_pop[i] = mix_netw(pop->tab[(int)(rand()*nb_const)], pop->tab[(int)(rand()*nb_const)]);
    
    for (; i<pop->size; ++i)
        new_pop[i] = gen_netw(pop->tab[0]->size);
    
    Population *p = malloc(sizeof(struct s_popu));
    p->tab = new_pop;
    p->size = pop->size;

    return p;
}
