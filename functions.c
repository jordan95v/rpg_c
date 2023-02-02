#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

void raise(char *error)
{
    printf("[ERROR] %s\n", error);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

SDL_Surface *loadImage(char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)
        raise("Couldn't loading the tileset, check the filename.");
    return surface;
}