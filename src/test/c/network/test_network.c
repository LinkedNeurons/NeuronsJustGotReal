#include <network.h>
#include <network/factory.h>
#include <stdio.h>
#include <CUnit/CUnit.h>

void network_test_feed() {
	NetworkFactoryData *data = INetworkFactory.init();

	srand(42);
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
	Network *net = INetworkFactory.build(data);
	Vector *input = NULL;
	Vector *output = NULL;
	IVector.init(2, (double[]) { 1, 0 }, &input);
	INetwork.feed(net, input, &output);

	CU_ASSERT_DOUBLE_EQUAL(output->tab[0], 0.7822, 0.001);
}

void network_test_gradient_descent() {
	NetworkFactoryData *data = INetworkFactory.init();

	srand(42);
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
	Network *net = INetworkFactory.build(data);
	Vector *input = NULL;
	Vector *desired = NULL;
	IVector.init(2, (double[]) { 1, 0 }, &input);
	IVector.init(1, (double[]) { 1 }, &desired);

	INetwork.gradient_descent(net, input, desired);
}
