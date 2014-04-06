#ifndef _NETWORK_FACTORY_H_
#define _NETWORK_FACTORY_H_

#include <stdlib.h>
#include "network.h"
#include "math/matrix.h"
#include "math/vector.h"

typedef struct NetworkLayer {
	size_t neurons;
	ActivationFunction function;
} NetworkLayer;

struct NFDNode {
	struct NFDNode *next;
	NetworkLayer layer;
};

typedef struct NetworkFactoryData {
	struct NFDNode *nodes;
	struct NFDNode *last;
	size_t layers;
} NetworkFactoryData;

NetworkFactoryData* network_factory_init();
void network_factory_destroy(NetworkFactoryData *data);
void network_factory_add_layer(NetworkFactoryData *data, const NetworkLayer layer);
Network *network_factory_build(NetworkFactoryData *data);
Network *network_factory_build_once(NetworkFactoryData *data);

struct NetworkFactoryModule {
	NetworkFactoryData* (*init)(void);
	void (*destroy)(NetworkFactoryData*);
	void (*add_layer)(NetworkFactoryData*, const NetworkLayer);
	Network* (*build)(NetworkFactoryData*);
	Network* (*build_once)(NetworkFactoryData*);
};

extern struct NetworkFactoryModule INetworkFactory;

#endif // _NETWORK_FACTORY_H_
