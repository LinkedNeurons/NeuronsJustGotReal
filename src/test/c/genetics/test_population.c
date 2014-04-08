#include <CUnit/CUnit.h>
#include <njgr/genetics/populations.h>

void population_test_generation() {
	NetworkFactoryData *data = INetworkFactory.init();

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

	long seed = 42;
	srand(seed);

	Population *p = IPopulation.create(5, data);
	IPopulation.init(p);

	Specimen *a = p->specimens[0];
	Specimen *b = p->specimens[1];
	Specimen *c = p->specimens[2];

	a->score = 1;
	b->score = 2;
	c->score = 3;

	IPopulation.next_generation(p, 3, 1);

	CU_ASSERT_EQUAL(c, p->specimens[0]);
	CU_ASSERT_EQUAL(b, p->specimens[1]);
	CU_ASSERT_EQUAL(a, p->specimens[2]);

	IPopulation.destroy(p);
}
