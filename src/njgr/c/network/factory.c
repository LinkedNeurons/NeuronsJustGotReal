#include "network/factory.h"

struct NetworkFactoryModule INetworkFactory = {
	.init       = &network_factory_init,
	.destroy    = &network_factory_destroy,
	.add_layer  = &network_factory_add_layer,
	.build      = &network_factory_build,
	.build_once = &network_factory_build_once
};

NetworkFactoryData* network_factory_init() {
	NetworkFactoryData* data = malloc(sizeof(NetworkFactoryData));
	data->nodes  = NULL;
	data->last   = NULL;
	data->layers = 0;
	return data;
}

void network_factory_destroy(NetworkFactoryData *data) {
	struct NFDNode *node = data->nodes;
	for (size_t i = 0; i < data->layers; ++i) {
		struct NFDNode *next = node->next;
		free(node);
		node = next;
	}
	free(data);
}

void network_factory_add_layer(NetworkFactoryData *data, const NetworkLayer layer) {
	struct NFDNode *node = malloc(sizeof(struct NFDNode));
	node->next  = NULL;
	node->layer = layer;
	if (!data->nodes) data->nodes = node;
	if ( data->last)  data->last->next = node;
	data->last = node;
	data->layers++;
}

double randweight() {
	return (double) rand() / RAND_MAX;
}

Network *network_factory_build(NetworkFactoryData *data) {
	Matrix* weights = malloc(sizeof(Matrix) * data->layers);
	Vector* biases  = malloc(sizeof(Vector) * data->layers);
	ActivationFunction *functions = malloc(sizeof(ActivationFunction) * data->layers);
	size_t i = 0;
	for (struct NFDNode *node = data->nodes; node && node->next; node = node->next) {
		Matrix *weight = weights + i;
		Vector *bias   = biases  + i;
		IMatrix.initf(node->next->layer.neurons, node->layer.neurons, (double (*)(size_t, size_t)) &randweight, &weight);
		IVector.initf(node->next->layer.neurons, (double (*)(size_t)) &randweight, &bias);
		functions[i] = node->layer.function;
		++i;
	}
	return INetwork.create(data->layers, weights, functions, biases);
}

Network *network_factory_build_once(NetworkFactoryData *data) {
	Network *network = network_factory_build(data);
	network_factory_destroy(data);
	return network;
}
