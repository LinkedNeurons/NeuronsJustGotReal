#ifndef _GENETIC_H_
#define _GENETIC_H_

#include <stdlib.h>
#include "math/vector.h"

struct s_popu {
        Vector** tab;
    size_t size;
};

typedef struct s_popu Population;
#endif
