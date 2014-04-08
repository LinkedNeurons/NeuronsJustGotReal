#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"

#include <njgr/network/factory.h>
#include <SDL/SDL_draw.h>
#include <SDL/SDL.h>

#define NEURON_SIZE   11
#define NEURON_MARGIN 50
#define LAYER_MARGIN  50

struct GraphicsModule IGraphics = {
    .draw_network = &graphics_draw_network
};

size_t imax(size_t a, size_t b) {
	return a > b ? a : b;
}

size_t graphics_get_max_layer_size(Network *net) {
	Matrix *m = net->weights;
	size_t max = 0;

	for(size_t i = 0; i < net->depth - 1; ++i, ++m) {
		max = imax(m->cols, max);
	}
	max = imax((m - 1)->rows, max);
	return max;
}

int graphics_get_position(Network *net, size_t layer, int neuron, int max) {
	Matrix *m = net->weights + layer;
	int layer_size = 0;

	if(layer == net->depth - 1) { m--; layer_size = m->rows; }
	else                        {      layer_size = m->cols; }

	return ((int)max / (layer_size + 1)) * neuron;
}

void getMinimalDimensions0(Network *net, int *width, int* height) {
	int maxNeurons = graphics_get_max_layer_size(net);
	getMinimalDimensions(net->depth, maxNeurons, width, height);
}

void getMinimalDimensions(int depth, int max_size, int *width, int* height) {
	*width  = depth    * (NEURON_SIZE + LAYER_MARGIN)  - LAYER_MARGIN ;
	*height = max_size * (NEURON_SIZE + NEURON_MARGIN) - NEURON_MARGIN;
}

int has_too_big_of_a_girth(SDL_Surface *surface, int depth, int max_size, int x, int y) {
	int width  = 0;
	int height = 0;

	getMinimalDimensions(depth, max_size, &width, &height);

	if(surface->w - x < width)  { return 0; }
	if(surface->h - y < height) { return 0; }
	return 1;     
}

void graphics_draw_network(Network *network, SDL_Surface *surface, int x, int y) {\
	int maxNeurons = graphics_get_max_layer_size(network);
	size_t max = maxNeurons * (NEURON_SIZE + NEURON_MARGIN);

	if(!has_too_big_of_a_girth(surface, network->depth, maxNeurons, x, y)) { return; }

	Matrix *m = network->weights;
	size_t i = 0;
	size_t layer_size = 0;

	for (; i < network->depth; ++i) {
		if(i == network->depth - 1) { m--; layer_size = m->rows; }
		else                    {      layer_size = m->cols; }

		int step = (int)max / (layer_size + 1);
		int k = step;

		for (size_t j = 0; j < layer_size; ++j) {
			Draw_FillCircle(surface,
					x + i * (NEURON_SIZE + LAYER_MARGIN),
					y + k, 10,
					0xFFFFFFFF);
			if(i == network->depth - 1) { continue; }

			for (size_t l = 0; l < m->rows; ++l) {
				Draw_Line(surface,
						x + i * (NEURON_SIZE + LAYER_MARGIN),
						y + k,
						x + (i + 1) * (NEURON_SIZE + LAYER_MARGIN),
						y + graphics_get_position(network, i + 1, l + 1, max),
						0xFFFFFFFF);
			}

			k += step;
		}
		++m;
	}
	SDL_Flip(surface);
}
