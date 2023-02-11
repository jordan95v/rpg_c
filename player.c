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

int checkCollision(Entity *player, Entity *enemy)
{
    if (player->x == enemy->x && player->y == enemy->y)
        return 0;
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

void moveEnemy(Map *map, Entity *enemy, Entity *player)
{
    if (enemy->y < player->y && checkMove(map, enemy->x, enemy->y + 1))
    {
        enemy->y++;
        (enemy->y == player->y && enemy->x == player->x) ? enemy->y-- : 0;
    }
    else if (enemy->y > player->y && checkMove(map, enemy->x, enemy->y - 1))
    {
        enemy->y--;
        (enemy->y == player->y && enemy->x == player->x) ? enemy->y++ : 0;
    }
    else
    {
        if (enemy->x < player->x && checkMove(map, enemy->x + 1, enemy->y))
        {
            enemy->x++;
            (enemy->x == player->x) ? enemy->x-- : 0;
            enemy->facing = RIGHT;
        }
        else if (enemy->x > player->x && checkMove(map, enemy->x - 1, enemy->y))
        {
            enemy->x--;
            (enemy->x == player->x) ? enemy->x++ : 0;
            enemy->facing = LEFT;
        }
    }
}
