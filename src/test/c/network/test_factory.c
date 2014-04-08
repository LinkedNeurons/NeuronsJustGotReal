#include <string.h>
#include <stdio.h>
#include <time.h>
#include <CUnit/CUnit.h>
#include <njgr/network/factory.h>

double randweights() {
	return (double) rand() / RAND_MAX;
}

void factory_test_build() {
	NetworkFactoryData *data = INetworkFactory.init();

	long seed = 42;
	srand(seed);
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 2,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 2,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 1,
			.function = ActivationFunctions.sigmoid
	});
	Network *net = INetworkFactory.build_once(data);

	double result_weights[9];
	memcpy(result_weights,     net->weights[0].arr, sizeof(double) * 4);
	memcpy(result_weights + 4, net->biases[0].tab,  sizeof(double) * 2);
	memcpy(result_weights + 6, net->weights[1].arr, sizeof(double) * 2);
	memcpy(result_weights + 8, net->biases[1].tab,  sizeof(double));
	
	srand(seed);
	double genweights[9] = { 
		randweights(), randweights(), randweights(),
		randweights(), randweights(), randweights(),
		randweights(), randweights(), randweights()
	};
	for (int i = 0; i < 9; ++i) {
		CU_ASSERT_EQUAL(result_weights[i], genweights[i]);
	}

	INetwork.destroy(net);
}
