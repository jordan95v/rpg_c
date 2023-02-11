#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "player.h"
#include "functions.h"

#define RIGHT 0
#define LEFT 8

void changeMap(SDL_Window *window, Map **map, char *filename, Entity *entity, int x, int y)
{
    entity->x = x;
    entity->y = y;
    SDL_FillRect(SDL_GetWindowSurface(window), NULL, 0);

    reloadMap(*map, filename);

    if (strstr(filename, "random"))
        randMap(*map);
}

int main(int argc, char **argv)
{
    // Map and Tileset loading.
    Map *map = loadMap("maps/main.txt");
    Entity *player = createEntity(10, 7, 100, "assets/char.png");

    Entity *enemy = createEntity(20, 7, 100, "assets/enemy.png");
    enemy->facing = RIGHT;

    // Window creation and everything SDL related.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;

    // if (showMenu(window) != 0)
    //     goto Quit;

    // Main loop.
    char map_name[50] = "main";
    SDL_Event event;
    for (int i = 0;;)
    {
        // Event management
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            printf("[INFO] Leaving program.\n");
            goto Quit;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                if (player->y > 0 && checkMove(map, player->x, player->y - 1))
                    player->y--;
                if (!checkCollision(player, enemy))
                    player->y++;
                break;

            case SDLK_DOWN:
                if (player->y < map->height_map - 1 && checkMove(map, player->x, player->y + 1))
                    player->y++;
                if (!checkCollision(player, enemy))
                    player->y--;
                break;

            case SDLK_RIGHT:
                if (player->x < map->width_map - 1 && checkMove(map, player->x + 1, player->y))
                    player->x++;
                if (!checkCollision(player, enemy))
                    player->x--;
                player->facing = RIGHT;
                break;

            case SDLK_LEFT:
                if (player->x > 0 && checkMove(map, player->x - 1, player->y))
                    player->x--;
                if (!checkCollision(player, enemy))
                    player->x++;
                player->facing = LEFT;
                break;

            case SDLK_SPACE:
                for (int j = 0; j < 8; j++)
                {
                    renderCharacter(window, player, map->tile_width, j, "attack");
                    SDL_UpdateWindowSurface(window);
                    SDL_Delay(16);
                }
                attack(player, enemy);
                break;
            }

            // Manage enemy attack
            if (enemy->will_attack)
            {
                attack(enemy, player);
                enemy->will_attack = 0;
            }
            else if (checkAttack(enemy, player))
                enemy->will_attack = 1;
            else
                moveEnemy(map, enemy, player);

            printf("health: %d\n", player->health);
            break;

        case SDL_MOUSEMOTION:
            continue;
            break;

        default:
            // This is used to determine which image to use.
            i++;
            if (i > 7)
                i = 0;
            break;
        }

        // Render the map and the character.
        renderMap(window, map);
        renderCharacter(window, player, map->tile_width, i, "normal");
        renderCharacter(window, enemy, map->tile_width, i, "normal");
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);

        // printf("x: %d y: %d\n", player->x, player->y);

        // Manage the main map event.
        if (strstr(map_name, "main"))
        {
            if (player->x == 11 && player->y == 22)
            {
                changeMap(window, &map, "maps/random/map.txt", player, 4, 12);
                strcpy(map_name, "random");
            }
            else if ((player->x == 8 || player->x == 7) && player->y == 13)
            {
                changeMap(window, &map, "maps/town_hall/map.txt", player, 19, 20);
                strcpy(map_name, "town_hall");
            }
            else if ((player->x == 23 || player->x == 24) && player->y == 22)
            {
                changeMap(window, &map, "maps/char_house/map.txt", player, 20, 5);
                strcpy(map_name, "house");
            }
        }

        // Manage the random map event.
        if (player->x == 3 && player->y == 12 && strstr(map_name, "random"))
        {
            changeMap(window, &map, "maps/main/map.txt", player, 10, 22);
            strcpy(map_name, "main");
        }

        // Manage the house map event.
        if ((player->x == 18 || player->x == 19 || player->x == 20) && player->y == 21 && strstr(map_name, "town_hall"))
        {
            changeMap(window, &map, "maps/main/map.txt", player, 8, 14);
            strcpy(map_name, "main");
        }

        // Manage the char house map event.
        if ((player->x == 19 || player->x == 20) && player->y == 4 && strstr(map_name, "house"))
        {
            changeMap(window, &map, "maps/main/map.txt", player, 24, 23);
            strcpy(map_name, "main");
        }
    }

Quit:
    // Destroy the window.
    if (window != NULL)
        SDL_DestroyWindow(window);

    // Free the map struct and exit gracefully.
    freeMap(map);
    freePlayer(player);
    SDL_Quit();
    return 0;
}