#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"

int init(SDL_Window **window, int w, int h)
{
    // Init the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    // Init the SDL_Image PNG.
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("Error with IMG_Init\n");
        return -1;
    }

    // Create the window.
    *window = SDL_CreateWindow("Zelda Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    return 0;
}

int main(int argc, char **argv)
{
    // Map and Tileset loading.
    Map *map = loadMap("map.txt");

    // Window creation.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;

    // Main loop.
    SDL_Event event;
    while (1)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                printf("Leaving the program.\n");
                break;
            }
        }
        renderMap(window, map);
    }

Quit:
    // Destroy the window.
    if (window != NULL)
        SDL_DestroyWindow(window);

    // Free the map struct and exit gracefully.
    freeMap(map);
    SDL_Quit();
    return 0;
}