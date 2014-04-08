#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>

#include <network/factory.h>

#define NEURON_SIZE   11
#define NEURON_MARGIN 50
#define LAYER_MARGIN  50

#define X_MARGIN 15
#define Y_MARGIN 15

Network *net;

size_t maxLayerDepth(Network *net) {
	Matrix *m = net->weights;
	size_t max = 0;

	for(size_t i = 0; i < net->depth; ++i) {
		max = m->cols > max ? m->cols : max;
		++m;
	}
	return max;
}


SDL_Surface *init() {
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
	net = INetworkFactory.build(data);


	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *display = SDL_SetVideoMode(700, 700, 32, SDL_HWSURFACE);
	SDL_FillRect(display, NULL, SDL_MapRGB(display->format, 44, 62, 80));
	SDL_Flip(display);
	SDL_WM_SetCaption("Neural network displayer", NULL);


	return display;
}

int getPosition(size_t layer, int neuron, int max) {
	Matrix *m = net->weights + layer;
	int layer_size = 0;

	if(layer == net->depth - 1) { m--; layer_size = m->rows; }
	else                        {      layer_size = m->cols; }

	return ((int)max / (layer_size + 1)) * neuron;

}

void draw(SDL_Surface *display) {
	int max = (int)maxLayerDepth(net) * (NEURON_SIZE + NEURON_MARGIN);

	Matrix *m = net->weights;
	size_t i = 0;
	size_t layer_size = 0;

	for (; i < net->depth; ++i) {
		if(i == net->depth - 1) { m--; layer_size = m->rows; }
		else                    {      layer_size = m->cols; }
		
		int step = (int)max / (layer_size + 1);
		int k = step;
		
		for (size_t j = 0; j < layer_size; ++j) {
			Draw_FillCircle(display,
            	X_MARGIN + i * (NEURON_SIZE + LAYER_MARGIN),
            	Y_MARGIN + k, 10,
             	0xFFFFFFFF);
			if(i == net->depth - 1) { continue; }

			for (size_t l = 0; l < m->rows; ++l) {
				Draw_Line(display,
	               X_MARGIN + i * (NEURON_SIZE + LAYER_MARGIN),
	               Y_MARGIN + k,
	               X_MARGIN + (i + 1) * (NEURON_SIZE + LAYER_MARGIN),
	               Y_MARGIN + getPosition(i + 1, l + 1, max),
	               0xFFFFFFFF);
			}

			k += step;
		}
		++m;
	}
	SDL_Flip(display);
}

void update() {

}

void destroy() {
	SDL_Quit();
}


// Events catcher
void catchKillEvent(int *killed) {
	SDL_Event event;
	SDL_WaitEvent(&event);
    switch(event.type) {
        case SDL_QUIT:    *killed = 1; break;
        case SDL_KEYDOWN:
        	switch(event.key.keysym.sym) {
        		case SDLK_ESCAPE: *killed = 1; break;
        		case SDLK_q:      *killed = 1; break;
        		default: break;
        	}
        break;
        default: return;
    }
}

int main() {
	printf("\n\n \t Display a neural network \n\n");
	SDL_Surface *display = init();
	draw(display);

	int killed = 0;
	while(!killed) {
		catchKillEvent(&killed);
		update();
	}

	destroy();
}