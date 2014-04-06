#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define WIDTH  700
#define HEIGHT 700
#define COLORS 32

#define GRID_SIZE 16

#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3

typedef struct {
	SDL_Surface *display;
	SDL_Surface *grid, *tile;
} Assets;

typedef struct {
	int grid[4][4];
	int lost; 
} Grid;

typedef struct {
	Grid *grids;
	int   size;
} Gameboard;

Assets *ass     = NULL;
Gameboard *game = NULL;

void init_gameboard(int grids) {
	game = malloc(sizeof(Gameboard));
	game->grids = malloc(grids * sizeof(Grid));

	for (int i = 0; i < grids; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k)
			{
				game->grids[i].grid[j][k] = 0;
			}
		}
		game->grids[i].lost = 0;

		int first = rand() % GRID_SIZE, second = 0;
		do {
			second = rand() % GRID_SIZE;
		} while (second == first);
		game->grids[i].grid[first  / 4][first  % 4] = 1;
		game->grids[i].grid[second / 4][second % 4] = 1;
	}
}

void moveColum(Grid *grid, int column, int up) {

	int start, direction, stop;
	if(up == 1) { start = 0; direction = 1;  stop = 3; }
	else 		{ start = 3; direction = -1; stop = 0; }

	for (int i = start; i != stop; i = i + direction) {
		if (i != stop && grid->grid[column][i + direction] == grid->grid[column][i]) {
			++grid->grid[column][i];
			grid->grid[column][i + direction] = 0;
		}
	}
	for (int i = start; i != stop; i = i + direction) {
		if(i != stop && grid->grid[column][i] == 0) {
			grid->grid[column][i] = grid->grid[column][i + direction];
			grid->grid[column][i + direction] = 0;
		}
	}
}

void moveLine(Grid *grid, int line, int left) {
	int start, direction, stop;
	if(left == 1) { start = 0; direction = 1;  stop = 3; }
	else 		{ start = 3; direction = -1; stop = 0; }

	for (int i = start; i != stop; i = i + direction) {
		if (i != stop && grid->grid[i + direction][line] == grid->grid[i][line]) {
			++grid->grid[i][line];
			grid->grid[i + direction][line] = 0;
		}
	}
	for (int i = start; i != stop; i = i + direction) {
		if(i != stop && grid->grid[i][line] == 0) {
			grid->grid[i][line] = grid->grid[i + direction][line];
			grid->grid[i + direction][line] = 0;
		}
	}
}

void moveGrid(int direction) {

}

void init() {

	ass = malloc(sizeof(Assets));
	init_gameboard(9);

	SDL_Init(SDL_INIT_VIDEO);
	ass->display = SDL_SetVideoMode(WIDTH, HEIGHT, COLORS, SDL_HWSURFACE);
	ass->grid = IMG_Load("assets/grid.png");
	ass->tile = IMG_Load("assets/tile.png");
	

	SDL_FillRect(ass->display, NULL, SDL_MapRGB(ass->display->format, 44, 62, 80));
	SDL_Flip(ass->display);

	SDL_WM_SetCaption("2048 Sample", NULL);
}


void draw() {
	for (int i = 0; i < game->size; ++i) {
		
	}
	SDL_Flip(ass->display);
}

void update() {

}

void destroy() {
	SDL_FreeSurface(ass->display);
	SDL_FreeSurface(ass->grid);
	SDL_FreeSurface(ass->tile);
	
	free(ass);

	free(game->grids);
	free(game);

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
	srand(42);
	init();

	int killed = 0;
	while(!killed) {
		catchKillEvent(&killed);
		draw();
		update();
	}

	destroy();
}