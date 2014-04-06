#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>

#define WIDTH  700
#define HEIGHT 700
#define COLORS 32

typedef struct {
	SDL_Surface *display;
	SDL_Surface *grid, tile;
} Assets;

Assets *ass = NULL; 

SDL_Surface *init() {

	ass = malloc(sizeof(Assets));

	SDL_Init(SDL_INIT_VIDEO);
	ass->display = SDL_SetVideoMode(WIDTH, HEIGHT, COLORS, SDL_HWSURFACE);
	

	SDL_FillRect(display, NULL, SDL_MapRGB(display->format, 44, 62, 80));
	SDL_Flip(display);

	SDL_WM_SetCaption("2048 Sample", NULL);


	return display;
}


void draw(SDL_Surface *display) {

	SDL_Flip(display);
}

void update() {

}

void destroy(SDL_Surface *display) {
	SDL_FreeSurface(display);
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

int main(int argc, char *argv) {
	printf("\n\n \t2048 Sample logs \n\n");
	SDL_Surface *display = init();

	int killed = 0;
	while(!killed) {
		catchKillEvent(&killed);
		draw(display);
		update();
	}

	destroy();
}