#include "njgr/math/matrix.h"
#include "njgr/math/vector.h"
#include "njgr/genetics/populations.h"

struct PopulationModule IPopulation = {
	.create          = &population_create,
	.init            = &population_init,
	.destroy         = &population_destroy,
	.next_generation = &population_next_generation
};

Population* population_create(const size_t size, NetworkFactoryData *data) {
	Population *p = NULL;
	new_struct(p, Population) {
		.size = size,
		.generation = 0,
		.specimens = malloc(sizeof(void*) * size),
		.factory_data = data
	};
	return p;
}

void population_init(Population *p) {
	for (size_t i = 0; i < p->size; ++i) {
		new_struct(p->specimens[i], Specimen) {
			.score = 0,
			.network = INetworkFactory.build(p->factory_data)
		};
	}
}

void population_destroy(Population *p) {
	for (size_t i = 0; i < p->size; ++i) {
		population_destroy_specimen(p->specimens[i]);
	}
	free(p->specimens);
	INetworkFactory.destroy(p->factory_data);
	free(p);
}

void population_destroy_specimen(Specimen *s) {
	INetwork.destroy(s->network);
	free(s);
}

void population_sort_fittest(Specimen **tab, size_t n) {
	if (n < 2) return;
	int p = tab[n / 2]->score;
	Specimen **l = tab;
	Specimen **r = tab + n - 1;
	while (l <= r) {
		if ((*l)->score > p) {
			l++;
			continue;
		}
		if ((*r)->score < p) {
			r--;
			continue;
		}
		Specimen *t = *l;
		*l++ = *r;
		*r-- = t;
	}
	population_sort_fittest(tab, r - tab + 1);
	population_sort_fittest(l, tab + n - l);
}

double population_pick_inheritance(size_t index, double c1, double c2) {
	return index / 10 % 2 ? c1 : c2;
}

void population_breed(Specimen *s1, Specimen *s2, Specimen **offspring) {
	size_t length = s1->network->depth - 1;

	Matrix *weights = malloc(sizeof(Matrix) * length);
	Matrix *w = weights, *w1 = s1->network->weights, *w2 = s2->network->weights;
	
	Vector *biases = malloc(sizeof(Vector) * length);
	Vector *v = biases, *v1 = s1->network->biases, *v2 = s2->network->biases;

	ActivationFunction *functions = malloc(sizeof(ActivationFunction) * length);
	ActivationFunction *f = functions, *f1 = s1->network->functions;

	for (size_t i = 0; i < length; ++i, ++w, ++v, ++f, ++w1, ++w2, ++v1, ++v2, ++f1) {
		double weight_array[w1->rows * w1->cols];
		for (size_t j = 0; j < (w1->rows * w1->cols); ++j) {
			weight_array[j] = population_pick_inheritance(j, w1->arr[j], w2->arr[j]);
		}
		IMatrix.init(w1->rows, w1->cols, weight_array, &w);

		double bias_array[v1->size];
		for (size_t j = 0; j < v1->size; ++j) {
			bias_array[j] = population_pick_inheritance(j, v1->tab[j], v2->tab[j]);
		}
		IVector.init(v1->size, bias_array, &v);

		*f = *f1;
	}

	new_struct(*offspring, Specimen) {
		.score = 0,
		.network = INetwork.create(s1->network->depth, weights, functions, biases)
	};
}

void population_next_generation(Population *population, size_t nb_keep, size_t nb_new) {
	population_sort_fittest(population->specimens, population->size);
	
	size_t i = nb_keep;
	Specimen **tab= population->specimens;
	for (; i < population->size - nb_new; ++i) {
		population_destroy_specimen(tab[i]);
		population_breed(tab[rand() % nb_keep], tab[rand() % nb_keep], &(tab[i]));
	}

	for (; i < population->size; ++i) {
		population_destroy_specimen(tab[i]);
		new_struct(tab[i], Specimen) {
			.score = 0,
			.network = INetworkFactory.build(population->factory_data)
		};
	}

	population->generation++;
}

