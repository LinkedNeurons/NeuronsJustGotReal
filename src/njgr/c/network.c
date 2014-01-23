#include <stdlib.h>
#include <stdio.h>

#include "network.h"

void print_layer(vector *layer, size_t n)
{
    printf("    layer %2lu : ", n);

    for(size_t i = 0; i<layer->size; ++i)
        printf("%5f ", at(layer, i));
    
    printf("/n");
}

void pretty_print(network *n)
{
    printf("Print network - %lu layers\n", n->depth);

    for(size_t i = 0; i<n->depth; ++i)
        print_layer(n->layers[i], i);

    printf("End print network\n");
}
