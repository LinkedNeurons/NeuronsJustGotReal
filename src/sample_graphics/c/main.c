#include <SDL/SDL.h>
#include <graphics.h>
#include <network.h>
#include <network/factory.h>

void init(SDL_Surface **display, Network **network) {
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
	*network = INetworkFactory.build_once(data);

	SDL_Init(SDL_INIT_VIDEO);
	*display = SDL_SetVideoMode(700, 700, 32, SDL_HWSURFACE);
	SDL_FillRect(*display, NULL, SDL_MapRGB((*display)->format, 44, 62, 80));
	SDL_Flip(*display);
	SDL_WM_SetCaption("Neural network renderer", NULL);
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
	SDL_Surface *display = NULL;
	Network *network = NULL;
	init(&display, &network);
	IGraphics.draw_network(network, display, 10, 10);

	int killed = 0;
	while(!killed) {
		catchKillEvent(&killed);
	}

	SDL_FreeSurface(display);
	INetwork.destroy(network);
	SDL_Quit();
}
