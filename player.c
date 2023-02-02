#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "functions.h"

Entity *createEntity(int x, int y, int health)
{
    Entity *entity = NULL;
    entity = malloc(sizeof(Entity));

    entity->x = x;
    entity->y = y;
    entity->health = health;
    strcpy(entity->orientation, "s");

    return entity;
}

void renderCharacter(SDL_Window *window, Entity *character, char *filename, int height, int width)
{
    SDL_Rect dest;
    dest.x = character->x * width;
    dest.y = character->y * height;
    SDL_Surface *surface = loadImage(filename);
    SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), &dest);
}