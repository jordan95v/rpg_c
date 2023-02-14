#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "functions.h"

#define RIGHT 0
#define LEFT 8
#define KEY_COST 50

void changeMap(SDL_Window *window, Map **map, char *filename, Entity *entity, int x, int y)
{
    entity->x = x;
    entity->y = y;
    SDL_FillRect(SDL_GetWindowSurface(window), NULL, 0);

    reloadMap(*map, filename);

    if (strstr(filename, "random"))
        randMap(*map);
}

int remove_element(Entity **array, int n, Entity *player)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        if (array[i]->health == 0)
        {
            player->coins += array[i]->coins;
            for (j = i; j < n - 1; j++)
            {
                array[j] = array[j + 1];
            }
            n--;
            i--;
        }
    }
    return n;
}

int main(int argc, char **argv)
{
    // Map and Tileset loading.
    Map *map = loadMap("maps/main.txt");
    char map_name[50] = "main";

    // Window creation and everything SDL related.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    // Entity creations.
    Entity *player = createEntity(10, 7, 5, "assets/char.png");
    Entity **enemies = NULL;
    SDL_Surface *enemy_surface = loadImage("assets/enemy.png");
    int enemies_number = 0;

    // Font creation and heart image load.
    TTF_Font *font = TTF_OpenFont("assets/Triforce.ttf", 24);
    SDL_Surface *heart = loadImage("assets/heart.png");
    SDL_Surface *store = loadImage("assets/store.png");
    SDL_Surface *key = loadImage("assets/key.png");

    // Show the menu.
    if (showMenu(window) != 0)
        goto Quit;
    int buyed = 0;

Main:
    // Main loop.
    int i, j = 0;
    SDL_Event event;
    for (i = 0;;)
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
                if (!checkCollision(player, enemies, enemies_number))
                    player->y++;
                break;

            case SDLK_DOWN:
                if (player->y < map->height_map - 1 && checkMove(map, player->x, player->y + 1))
                    player->y++;
                if (!checkCollision(player, enemies, enemies_number))
                    player->y--;
                break;

            case SDLK_RIGHT:
                if (player->x < map->width_map - 1 && checkMove(map, player->x + 1, player->y))
                    player->x++;
                if (!checkCollision(player, enemies, enemies_number))
                    player->x--;
                player->facing = RIGHT;
                break;

            case SDLK_LEFT:
                if (player->x > 0 && checkMove(map, player->x - 1, player->y))
                    player->x--;
                if (!checkCollision(player, enemies, enemies_number))
                    player->x++;
                player->facing = LEFT;
                break;

            case SDLK_SPACE:
                for (j = 0; j < 8; j++)
                {
                    renderCharacter(window_surface, player, map->tile_width, j, "attack");
                    SDL_UpdateWindowSurface(window);
                    SDL_Delay(16);
                }
                for (j = 0; j < enemies_number; j++)
                {
                    attack(player, enemies[j]);
                }
                enemies_number = remove_element(enemies, enemies_number, player);
                if (player->have_key && player->x == 18 && player->y == 2)
                {
                    system("open assets/c.pdf");
                    map->layers[3].schema[18][1] = 416;
                    map->layers[3].schema[18][0] = 400;
                }
                break;
            }

            moveEnemy(map, enemies, enemies_number, player);
            for (j = 0; j < enemies_number; j++)
            {
                // Manage enemy attack
                if (enemies[j]->will_attack)
                {
                    attack(enemies[j], player);
                    enemies[j]->will_attack = 0;
                }
                else if (checkAttack(enemies[j], player))
                    enemies[j]->will_attack = 1;
            }
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
        SDL_FillRect(window_surface, NULL, 0);
        renderMap(window_surface, map);
        renderCharacter(window_surface, player, map->tile_width, i, "normal");
        if (enemies != NULL)
        {
            for (j = 0; j < enemies_number; j++)
            {
                if (enemies[j] != NULL)
                    renderCharacter(window_surface, enemies[j], map->tile_width, i, "normal");
            }
        }

        // UI rendering.
        renderCoinsUI(font, window_surface, player->coins);
        renderHeartUI(font, heart, window_surface, player->health);
        if (player->have_key)
            renderKeyUI(window_surface, key);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);

        if (player->health == 0)
        {
            gameOver(window_surface, font);
            SDL_UpdateWindowSurface(window);
            while (1)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                        goto Quit;

                    if (event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        for (j = 0; j < enemies_number; j++)
                        {
                            enemies[j]->health = 0;
                        }
                        enemies_number = remove_element(enemies, enemies_number, player);
                        changeMap(window, &map, "maps/main/map.txt", player, 10, 7);
                        strcpy(map_name, "main");
                        player->health = 5;
                        player->coins = 0;
                        goto Main;
                    }
                }
            }
        }

        // Manage the main map event.
        if (strstr(map_name, "main"))
        {
            if (player->x == 11 && player->y == 22)
            {
                srand(time(0));
                int max_enemies = rand() % 3;

                changeMap(window, &map, "maps/random/map.txt", player, 4, 12);
                strcpy(map_name, "random");
                for (int k = 10; k < map->width_map; k++)
                {
                    for (j = 0; j < map->height_map; j++)
                    {
                        if (rand() % 50 == 1)
                        {
                            if (map->layers[0].schema[k][j] == map->dead_zone_number && enemies_number < max_enemies + 1)
                            {
                                enemies = (Entity **)realloc(enemies, sizeof(Entity *) * enemies_number + 1);
                                if (enemies == NULL)
                                {
                                    raise("Error while generating enemies.");
                                    goto Quit;
                                }
                                enemies[enemies_number] = createEnemy(k, j, 3, enemy_surface, rand() % 4 + 1);
                                enemies[enemies_number]->facing = LEFT;
                                enemies_number++;
                            }
                        }
                    }
                }
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
            else if ((player->x == 13 || player->x == 14) && player->y == 7)
            {
                if (buyed == 0)
                {
                    SDL_BlitSurface(store, NULL, window_surface, NULL);
                    SDL_UpdateWindowSurface(window);

                    while (1)
                    {
                        SDL_WaitEvent(&event);

                        switch (event.type)
                        {
                        case SDL_QUIT:
                            goto Quit;
                            break;

                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym)
                            {
                            case SDLK_o:
                                if (player->coins >= KEY_COST)
                                {
                                    buyed = 1;
                                    player->have_key = 1;
                                    player->coins -= KEY_COST;
                                }
                                player->y++;
                                goto Main;
                            case SDLK_n:
                                printf("et bah non\n");
                                buyed = 0;
                                player->y++;
                                goto Main;
                            }
                            break;
                        }
                    }
                }
            }
        }

        // Manage the random map event.
        if (player->x == 3 && player->y == 12 && strstr(map_name, "random") && enemies_number == 0)
        {
            player->health = 5;
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
    TTF_CloseFont(font);
    SDL_FreeSurface(store);
    SDL_FreeSurface(heart);
    SDL_FreeSurface(key);
    for (j = 0; j < enemies_number; j++)
        freePlayer(enemies[j]);
    SDL_Quit();
    return 0;
}