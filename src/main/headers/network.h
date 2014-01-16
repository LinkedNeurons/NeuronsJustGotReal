
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>

typedef network *network_s; 

struct network_s {
	size_t  depth;
	vector_t *layer;
}

#endif
