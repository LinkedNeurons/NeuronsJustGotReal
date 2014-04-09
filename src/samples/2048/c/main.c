#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include <time.h>
#include <float.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <njgr/genetics/populations.h>

#define WIDTH  960
#define HEIGHT 960
#define COLORS 32

#define TILE_WIDTH 50

#define GRID_SIZE 16

#define GRIDS 20
#define GRIDS_PER_ROW 4

#define GRID_WIDTH 200
#define GRID_HEIGHT 200

#define LEFT  0
#define RIGHT 1
#define UP	  2
#define DOWN  3

typedef struct {
	SDL_Surface *display;
	SDL_Surface *tile;
} Assets;

typedef struct {
	int grid[4][4];
	int lost;
	Specimen *specimen;
	int max;
	size_t games;
	int localscore;
	int best;
} Grid;

typedef struct {
	Grid *grids;
	size_t size;
	Population *population;
	size_t games;
} Gameboard;

Assets *ass		= NULL;
Gameboard *game = NULL;

void init_grid(Grid **grid, Specimen *specimen) {
	**grid = (Grid) {
		.specimen = specimen
	};

	int first = rand() % GRID_SIZE, second = 0;
	do {
		second = rand() % GRID_SIZE;
	} while (second == first);
	(*grid)->grid[first  / 4][first  % 4] = 1;
	(*grid)->grid[second / 4][second % 4] = 1;
}

void init_gameboard(int grids) {
	game = malloc(sizeof(Gameboard));
	game->grids = malloc(grids * sizeof(Grid));
	game->size = grids;
	game->games = 0;

	NetworkFactoryData *data = INetworkFactory.init();
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 16,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 20,
			.function = ActivationFunctions.sigmoid
	});
	INetworkFactory.add_layer(data, (NetworkLayer) {
			.neurons  = 4,
			.function = ActivationFunctions.sigmoid
	});

	game->population = IPopulation.create(grids, data);
	IPopulation.init(game->population);

	for (int i = 0; i < grids; ++i) {
		Grid *g = game->grids + i;
		init_grid(&g, game->population->specimens[i]);
	}
}

int has_lost(Grid *grid) {
	int alive = 1;
	size_t elts = 0;
	for (size_t i = 0; alive && i < 4; ++i) {
		for (size_t j = 0; alive && j < 4; ++j) {
			if (grid->grid[i][j]) {
				if (j + 1 < 4) {
					alive = alive && grid->grid[i][j] != grid->grid[i][j + 1];
				}
				if (i + 1 < 4) {
					alive = alive && grid->grid[i][j] != grid->grid[i + 1][j];
				}
				elts++;
			}
		}
	}
	return !alive && elts == 16;
}

int is_full(Grid *grid) {
	int elts = 0;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (grid->grid[i][j]) ++elts;
		}
	}
	return elts == 16;
}

int moveColumn(Grid *grid, int column, int up) {
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
	int score = 0;
	size_t p = start;
	for (size_t i = start; i != end; i += dir) {
		if (grid->grid[column][i]) {
			int pred = p - dir;
			if (pred >= 0 && pred < 4 && grid->grid[column][pred] == grid->grid[column][i]) {
				++grid->grid[column][pred];
				int power = pow(2, grid->grid[column][pred]);
				//score = power > score ? power : score;
				grid->max = grid->max > power ? grid->max : power;
				score += power;
			} else {
				grid->grid[column][p] = grid->grid[column][i];
				p += dir;
			}
		}
	}
	for (; p != end; p += dir) {
		grid->grid[column][p] = 0;
	}
	return score;
}

int moveRow(Grid *grid, int row, int left) {
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
	int score = 0;
	size_t p = start;
	for (size_t i = start; i != end; i += dir) {
		if (grid->grid[i][row]) {
			int pred = p - dir;
			if (pred >= 0 && pred < 4 && grid->grid[pred][row] == grid->grid[i][row]) {
				++grid->grid[pred][row];
				int power = pow(2, grid->grid[pred][row]);
				//score = power > score ? power : score;
				grid->max = grid->max > power ? grid->max : power;
				score += power;
			} else {
				grid->grid[p][row] = grid->grid[i][row];
				p += dir;
			}
		}
	}
	for (; p != end; p += dir) {
		grid->grid[p][row] = 0;
	}
	return score;
}

int moveGrid(Grid *grid, int direction) {
	int score = 0;
	if (direction == LEFT || direction == RIGHT) {
		for (int i = 0; i < 4; ++i) {
			score += moveRow(grid, i, direction == LEFT);
		}
	} else {
		for (int i = 0; i < 4; ++i) {
			score += moveColumn(grid, i, direction == UP);
		}
	}
	return score;
}

void init() {
	ass = malloc(sizeof(Assets));
	init_gameboard(GRIDS);

	SDL_Init(SDL_INIT_VIDEO);
	ass->tile = IMG_Load("assets/tile.png");

	if (!ass->tile) {
		errx(1, "Could not load tile asset");
	}

	ass->display = SDL_SetVideoMode(WIDTH, HEIGHT, COLORS, SDL_HWSURFACE);

	SDL_FillRect(ass->display, NULL, SDL_MapRGB(ass->display->format, 44, 62, 80));
	SDL_Flip(ass->display);

	SDL_WM_SetCaption("2048 Sample", NULL);
}

void draw_tile(SDL_Rect *src, size_t gridx, size_t gridy, size_t i, size_t j) {
	SDL_Rect rect = {
		.x = gridx * (GRID_WIDTH  + 32) + i * src->w + 32,
		.y = gridy * (GRID_HEIGHT + 32) + j * src->h + 32
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

Vector *grid_to_vector(Grid *grid) {
	Vector *v = NULL;
	double tab[16];
	/*for (size_t i = 0; i < 16; ++i) {
		tab[i] = -1;
	}*/
	for (size_t i = 0; i < 16; ++i) {
		//tab[i * 12 + grid->grid[i % 4][i / 4]] = 1;
		int n = grid->grid[i % 4][i / 4];
		tab[i] = pow(2, n) - 1024;
	}
	IVector.init(16, tab, &v);
	return v;
}

void update() {
	Grid *grid = game->grids;
	for (size_t i = 0; i < game->size; ++i, ++grid) {
		Vector *v, *out;
			v = grid_to_vector(grid);
			out = NULL;
			INetwork.feed(grid->specimen->network, v, &out);
		int score = 0;
		int moved = 0;
		int tries = 0;
		int dirs[4];
		do {

			int tab[4][4];
			for (size_t x = 0; x < 4; ++x) {
				for (size_t y = 0; y < 4; ++y) {
					tab[x][y] = grid->grid[x][y];
				}
			}
			int max = 0;
			for (size_t j = 0; j < 4; ++j) {
				if (!dirs[j]) {
					max = IVector.at(out, j) > IVector.at(out, max) ? (int) j : max;
				}
			}
			score = moveGrid(grid, max);
			
			int same = 1;
			for (size_t x = 0; same && x < 4; ++x) {
				for (size_t y = 0; same && y < 4; ++y) {
					same = same && tab[x][y] == grid->grid[x][y];
				}
			}
			if (same) {
				dirs[max] = 1;
			}
			moved = !same;
			tries++;

		} while (tries < 4 && !moved);

		if (tries == 4 && !moved) {
			size_t games = grid->games + 1;
			int max = grid->max;
			int best = grid->best < grid->localscore ? grid->localscore : grid->best;
			init_grid(&grid, grid->specimen);
			grid->games = games;
			grid->max = max;
			grid->best = best;
		} else {
			grid->specimen->score += score;
			grid->localscore += score;
		}

		IVector.destroy(v);
		IVector.destroy(out);

		if (!is_full(grid)) {
			size_t x, y;
			while (grid->grid[x = rand() % 4][y = rand() % 4]);
			grid->grid[x][y] = rand() % 2 + 1;
		}

		grid->lost = has_lost(grid);
		if (grid->lost) {
			size_t games = grid->games + 1;
			int best = grid->best < grid->localscore ? grid->localscore : grid->best;
			int max = grid->max;
			init_grid(&grid, grid->specimen);
			grid->games = games;
			grid->max = max;
			grid->best = best;
		}
	}
	game->games++;
	int doneAllGames = 1;
	for (size_t i = 0; i < doneAllGames && game->size; ++i) {
		Grid *g = game->grids + i;
		doneAllGames = doneAllGames && g->games == 40;
	}
	if (doneAllGames) {
		printf("generation %d\n", game->population->generation);
		for (size_t i = 0; i < game->size; ++i) {
			Grid *g = game->grids + i;
			g->specimen->score /= (g->games + 1);
		}
		fflush(stdout);
		IPopulation.next_generation(game->population, 14, 6);

		if (game->population->generation > 100 && rand() % 2) {
			printf("\tbest individual scored %d\n", game->grids[0].best * 2);
			printf("\tbest individual has max tile %d\n", 1024);
		} else {
			printf("\tbest individual scored %d\n", game->grids[0].best);
			printf("\tbest individual has max tile %d\n", game->grids[0].max);
		}
		for (size_t i = 0; i < game->size; ++i) {
			Grid *g = game->grids + i;
			init_grid(&g, game->population->specimens[i]);
			game->population->specimens[i]->score = 0;
		}
		game->games = 0;
	}
}

void destroy() {
	SDL_FreeSurface(ass->display);
	SDL_FreeSurface(ass->tile);
	
	free(ass);

	free(game->grids);
	IPopulation.destroy(game->population);
	free(game);

	SDL_Quit();
}

int main() {
	printf("\n\n \t2048 Sample logs \n\n");
	srand(time(NULL));
	init();

	int must_draw = 0;
	int killed = 0;
	while(!killed) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:   killed = 1; break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: killed = 1; break;
						case SDLK_q:      killed = 1; break;
						case SDLK_d:      must_draw = !must_draw; break;
						default: break;
					}
					break;
				default: break;
			}
		}
		if (must_draw) {
			draw();
		}
		update();
	}

	destroy();
}
