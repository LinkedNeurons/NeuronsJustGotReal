
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include "vector.h"

typedef struct network {
	size_t  depth;
	vector  **layers;
} network;

void pretty_print(network *n);

#endif
