#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "functions.h"

Entity *createEntity(int x, int y, int health, char *filename)
{
    Entity *entity = NULL;
    entity = malloc(sizeof(Entity));

    entity->x = x;
    entity->y = y;
    entity->health = health;
    entity->tileset = loadImage(filename);
    entity->facing = 0;

    return entity;
}

void renderCharacter(SDL_Window *window, Entity *character, int size, int y, int shift, char *mode)
{
    SDL_Rect src, dest;
    src.w = size;
    src.h = size * 2;
    src.y = y;
    src.x = size * (3 * character->facing + shift);

    dest.x = character->x * size;
    dest.y = character->y * size;

    // This is so the attack render correctly, because it is an 32x48 tile !
    if (strcmp(mode, "attack") == 0)
    {
        src.w = size * 3;
        src.y = y * size;
        src.x = size * (3 * shift);
        dest.x = (character->x - 1) * size;
    }

    SDL_BlitSurface(character->tileset, &src, SDL_GetWindowSurface(window), &dest);
}
