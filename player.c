#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "functions.h"

#define RIGHT 0
#define LEFT 8

Entity *createEntity(int x, int y, int health, char *filename)
{
    Entity *entity = NULL;
    entity = malloc(sizeof(Entity));

    entity->x = x;
    entity->y = y;
    entity->health = health;
    entity->tileset = loadImage(filename);
    entity->facing = 0;
    entity->will_attack = 0;

    return entity;
}

Entity *createEnemy(int x, int y, int health, SDL_Surface *surface)
{
    Entity *entity = NULL;
    entity = malloc(sizeof(Entity));

    entity->x = x;
    entity->y = y;
    entity->health = health;
    entity->tileset = surface;
    entity->facing = 0;
    entity->will_attack = 0;

    return entity;
}

int checkCollision(Entity *player, Entity **enemies, int enemies_number)
{
    if (enemies == NULL)
        return 1;

    for (int i = 0; i < enemies_number; i++)
    {
        if (player->x == enemies[i]->x && player->y == enemies[i]->y)
            return 0;
    }
    return 1;
}

void renderCharacter(SDL_Window *window, Entity *character, int size, int shift, char *mode)
{
    SDL_Rect src, dest;
    src.w = size;
    src.h = size;
    src.y = 0;
    src.x = size * (character->facing + shift);

    if (strcmp(mode, "attack") == 0)
    {
        src.y = size;
    }

    dest.x = character->x * size;
    dest.y = character->y * size;

    SDL_BlitSurface(character->tileset, &src, SDL_GetWindowSurface(window), &dest);
}

void freePlayer(Entity *entity)
{
    SDL_FreeSurface(entity->tileset);
    free(entity);
}

void attack(Entity *attacker, Entity *victim)
{
    switch (attacker->facing)
    {
    case RIGHT:
        if (attacker->x + 1 == victim->x)
        {
            victim->health -= 25;
            victim->x++;
        }
    case LEFT:
        if (attacker->x - 1 == victim->x)
        {
            victim->health -= 25;
            victim->x--;
        }
    }
}

int checkAttack(Entity *attacker, Entity *victim)
{
    if (attacker->y == victim->y)
    {
        switch (attacker->facing)
        {
        case RIGHT:
            if (attacker->x + 1 == victim->x)
                return 1;
        case LEFT:
            if (attacker->x - 1 == victim->x)
                return 1;
        }
    }
    return 0;
}

void moveEnemy(Map *map, Entity **enemies, int enemies_number, Entity *player)
{
    int i, j, k;
    char orientation;

    for (i = 0; i < enemies_number; i++)
    {
        if (enemies[i]->will_attack != 0)
            continue;

        for (j = 0; j < enemies_number; j++)
        {
            if (i == j)
                continue;

            if (enemies[i]->x < player->x && checkMove(map, enemies[i]->x + 1, enemies[i]->y))
            {
                enemies[i]->x++;
                (enemies[i]->x == player->x) ? enemies[i]->x-- : 0;
                orientation = 'r';
                enemies[i]->facing = RIGHT;
            }
            else if (enemies[i]->x > player->x && checkMove(map, enemies[i]->x - 1, enemies[i]->y))
            {
                enemies[i]->x--;
                (enemies[i]->x == player->x) ? enemies[i]->x++ : 0;
                orientation = 'l';
                enemies[i]->facing = LEFT;
            }
            else
            {
                if (enemies[i]->y < player->y && checkMove(map, enemies[i]->x, enemies[i]->y + 1))
                {
                    enemies[i]->y++;
                    (enemies[i]->y == player->y && enemies[i]->x == player->x) ? enemies[i]->y-- : 0;
                    orientation = 'd';
                }
                else if (enemies[i]->y > player->y && checkMove(map, enemies[i]->x, enemies[i]->y - 1))
                {
                    enemies[i]->y--;
                    (enemies[i]->y == player->y && enemies[i]->x == player->x) ? enemies[i]->y++ : 0;
                    orientation = 'u';
                }
            }

            for (k = 0; k < enemies_number; k++)
            {
                if (k == i)
                    continue;

                if (enemies[i]->x == enemies[k]->x && enemies[i]->y == enemies[k]->y)
                {
                    if (orientation == 'r')
                        enemies[i]->x++;
                    else if (orientation == 'l')
                        enemies[i]->x--;
                    else if (orientation == 'u')
                        enemies[i]->y++;
                    else if (orientation == 'd')
                        enemies[i]->y--;
                }
            }
        }
    }
}
