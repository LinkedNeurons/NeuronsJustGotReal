#include "njgr/math/matrix.h"
#include "njgr/math/vector.h"
#include "njgr/genetics/populations.h"

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

void population_destroy(Population *p) {
	for (size_t i = 0; i < p->size; ++i) {
		population_destroy_specimen(p->specimens[i]);
	}
	INetworkFactory.destroy(p->factory_data);
	free(p);
}

void population_destroy_specimen(Specimen *s) {
	INetwork.destroy(s->network);
	free(s);
}

void quick_sort(Specimen **tab, size_t n) {
	int p = tab[n / 2]->score;
	Specimen **l = tab;
	Specimen **r = tab + n - 1;
	while (l <= r) {
		if ((*l)->score < p) {
			l++;
			continue;
		}
		if ((*r)->score > p) {
			r--;
			continue;
		}
		Specimen *t = *l;
		*l++ = *r;
		*r-- = t;
	}
	quick_sort(tab, r - tab + 1);
	quick_sort(l, tab + n - l);
}

void population_sort_fittest(Population *population) {
	quick_sort(population->specimens, population->size);
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

	for (size_t i = 0; i < length; ++i, ++w) {
		double weight_array[w1->rows * w1->cols];
		for (size_t j = 0; j < (w1->rows * w1->cols); ++j) {
			weight_array[j] = population_pick_inheritance(j, w1->arr[j], w2->arr[j]);
		}
		IMatrix.init(w1->rows, w1->cols, weight_array, &w);

		double bias_array[v->size];
		for (size_t j = 0; j < v->size; ++j) {
			v->tab[j] = population_pick_inheritance(j, v1->tab[j], v2->tab[j]);
		}
		IVector.init(v->size, bias_array, &v);
	}

	new_struct(*offspring, Specimen) {
		.score = 0,
		.network = INetwork.create(s1->network->depth, weights, functions, biases)
	};
}

void population_next_generation(Population *population, size_t nb_keep, size_t nb_new) {
	population_sort_fittest(population);
	
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

