#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <njgr/genetics/populations.h>

#define WIDTH  700
#define HEIGHT 700
#define COLORS 32

#define TILE_WIDTH 50

#define GRID_SIZE 16

#define GRIDS 9
#define GRIDS_PER_ROW 3

#define GRID_WIDTH 200
#define GRID_HEIGHT 200

#define LEFT  0
#define RIGHT 1
#define UP	  2
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
	Population *population;
} Gameboard;

Assets *ass		= NULL;
Gameboard *game = NULL;

void init_gameboard(int grids) {
	game = malloc(sizeof(Gameboard));
	game->grids = malloc(grids * sizeof(Grid));
	game->size = grids;

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

void moveColumn(Grid *grid, int column, int up) {
	size_t start, end, dir;
	if (up) {
		start = 0;
		end = 4;
		dir = 1;
	} else {
		start = 3;
		end = -1;
		dir = -1;
	}
	size_t p = start;
	for (size_t i = start; i != end; i += dir) {
		if (grid->grid[column][i]) {
			size_t pred = p - dir;
			if (pred >= 0 && pred < 4 && grid->grid[column][pred] == grid->grid[column][i]) {
				++grid->grid[column][pred];
			} else {
				grid->grid[column][p] = grid->grid[column][i];
				p += dir;
			}
		}
	}
	for (; p != end; p += dir) {
		grid->grid[column][p] = 0;
	}
}

void moveRow(Grid *grid, int row, int left) {
	size_t start, end, dir;
	if (left) {
		start = 0;
		end = 4;
		dir = 1;
	} else {
		start = 3;
		end = -1;
		dir = -1;
	}
	size_t p = start;
	for (size_t i = start; i != end; i += dir) {
		if (grid->grid[i][row]) {
			size_t pred = p - dir;
			if (pred >= 0 && pred < 4 && grid->grid[pred][row] == grid->grid[i][row]) {
				++grid->grid[pred][row];
			} else {
				grid->grid[p][row] = grid->grid[i][row];
				p += dir;
			}
		}
	}
	for (; p != end; p += dir) {
		grid->grid[p][row] = 0;
	}
}

void moveGrid(Grid *grid, int direction) {
	if (direction == LEFT || direction == RIGHT) {
		for (int i = 0; i < 4; ++i) {
			moveRow(grid, i, direction == LEFT);
		}
	} else {
		for (int i = 0; i < 4; ++i) {
			moveColumn(grid, i, direction == UP);
		}
	}
	size_t x, y;
	while (!grid->grid[x = rand() % 4][y = rand() % 4]) {
		grid->grid[x][y] = rand() % 2 + 1;
	}
}

void init() {

	ass = malloc(sizeof(Assets));
	init_gameboard(GRIDS);

	SDL_Init(SDL_INIT_VIDEO);
	ass->grid = IMG_Load("assets/grid.png");
	ass->tile = IMG_Load("assets/tile.png");

	if (!ass->tile) {
		errx(1, "Could not load tile asset");
	}

	ass->display = SDL_SetVideoMode(WIDTH, HEIGHT, COLORS, SDL_HWSURFACE);
	
	NetworkFactoryData *data = INetworkFactory.init();
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 16 * 11,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 30,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 4,
			.function = ActivationFunctions.sigmoid
	});

	long seed = 42;
	srand(seed);

	Population *p = IPopulation.create(5, data);
	IPopulation.init(p);

	SDL_FillRect(ass->display, NULL, SDL_MapRGB(ass->display->format, 44, 62, 80));
	SDL_Flip(ass->display);

	SDL_WM_SetCaption("2048 Sample", NULL);
}

void draw_tile(SDL_Rect *src, size_t gridx, size_t gridy, size_t i, size_t j) {
	SDL_Rect rect = {
		.x = gridx * GRID_WIDTH  + i * src->w,
		.y = gridy * GRID_HEIGHT + j * src->h
	};
	SDL_BlitSurface(ass->tile, src, ass->display, &rect);
}

void draw() {
	for (int i = 0; i < game->size; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			int t = game->grids[i].grid[j % 4][j / 4];
			SDL_Rect src = {
				.x = t * TILE_WIDTH,
				.w = TILE_WIDTH,
				.h = TILE_WIDTH
			};
			draw_tile(&src, i % GRIDS_PER_ROW, i / GRIDS_PER_ROW, j % 4, j / 4);
		}
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
	IPopulation.destroy(game->population);
	free(game);

	SDL_Quit();
}


// Events catcher
void catchKillEvent(int *killed) {
	SDL_Event event;
	SDL_WaitEvent(&event);
	switch(event.type) {
		case SDL_QUIT:            *killed = 1; break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE: *killed = 1; break;
				case SDLK_q:      *killed = 1; break;
				case SDLK_h:      moveGrid(game->grids, LEFT); break;
				case SDLK_j:      moveGrid(game->grids, DOWN); break;
				case SDLK_k:      moveGrid(game->grids, UP); break;
				case SDLK_l:      moveGrid(game->grids, RIGHT); break;
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
