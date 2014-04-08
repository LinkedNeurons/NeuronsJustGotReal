#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <njgr/network/network.h>
#include <SDL/SDL.h>

void graphics_draw_network(Network *network, SDL_Surface *surface, int x, int y);
void getMinimalDimensions0(Network *net, int *width, int* height);
void getMinimalDimensions(int depth, int max_size, int *width, int* height);

struct GraphicsModule {
    void (*draw_network)(Network*, SDL_Surface*, int, int);
};

extern struct GraphicsModule IGraphics;

#endif // GRAPHICS_H_
