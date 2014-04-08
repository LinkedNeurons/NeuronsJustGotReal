#ifndef POPULATIONS_H_
#define POPULATIONS_H_

#include "njgr/network/network.h"
#include "njgr/network/factory.h"

typedef struct {
	Network *network;
	int score;
} Specimen;

typedef struct {
	size_t size;
	size_t generation;
	Specimen **specimens;
	NetworkFactoryData *factory_data;
} Population;

Population* population_create(const size_t, NetworkFactoryData*);
void population_init(Population *p);
void population_destroy(Population *p);
void population_destroy_specimen(Specimen *s);
void population_sort_fittest(Specimen**, size_t);
double population_pick_inheritance(size_t index, double c1, double c2);
void population_breed(Specimen *s1, Specimen *s2, Specimen **offspring);
void population_next_generation(Population *population, size_t nb_keep, size_t nb_new);

struct PopulationModule {
	Population* (*create)(const size_t, NetworkFactoryData*);
	void (*init)(Population*);
	void (*destroy)(Population*);
	void (*next_generation)(Population*, size_t, size_t);
};

extern struct PopulationModule IPopulation;

#endif // POPULATIONS_H_
