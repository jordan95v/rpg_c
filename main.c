#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "player.h"
#include "functions.h"

#define DOWN 0
#define RIGHT 2
#define UP 4
#define LEFT 6

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

int showMenu(SDL_Window *window)
{
    SDL_Rect menu = {370, 250, 200, 50};
    SDL_Surface *backgroundMenu = loadImage("assets/menu.png");
    SDL_Surface *backgroundBtn = loadImage("assets/button.png");

    SDL_Event event;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return -1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= menu.x && x <= menu.x + menu.w && y >= menu.y && y <= menu.y + menu.h)
                {
                    SDL_FreeSurface(backgroundMenu);
                    SDL_FreeSurface(backgroundBtn);
                    return 0;
                }
            }
        }
        SDL_BlitSurface(backgroundMenu, NULL, SDL_GetWindowSurface(window), NULL);
        SDL_BlitSurface(backgroundBtn, NULL, SDL_GetWindowSurface(window), &menu);
        SDL_UpdateWindowSurface(window);
    }
}

int main(int argc, char **argv)
{
    // Map and Tileset loading.
    Map *map = loadMap("map.txt");
    Entity *player = createEntity(10, 7, 100, "assets/char.png");

    // Window creation and everything SDL related.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;

    if (showMenu(window) != 0)
        goto Quit;

    int i, y = 0;

    // Main loop.
    SDL_Event event;
    while (1)
    {
        // Event management
        if (SDL_PollEvent(&event))
        {
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
                    player->facing = UP;
                    break;

                case SDLK_DOWN:
                    if (player->y < map->height_map - 2 && checkMove(map, player->x, player->y + 1))
                        player->y++;
                    player->facing = DOWN;
                    break;

                case SDLK_RIGHT:
                    if (player->x < map->width_map - 2 && checkMove(map, player->x + 1, player->y))
                        player->x++;
                    player->facing = RIGHT;
                    break;

                case SDLK_LEFT:
                    if (player->x > 0 && checkMove(map, player->x - 1, player->y))
                        player->x--;
                    player->facing = LEFT;
                    break;

                case SDLK_SPACE:
                    switch (player->facing)
                    {
                    case UP:
                        y = 12;
                        break;
                    case DOWN:
                        y = 8;
                        break;
                    case LEFT:
                        y = 14;
                        break;
                    case RIGHT:
                        y = 10;
                        break;
                    }
                    for (int j = 0; j < 3; j++)
                    {
                        renderCharacter(window, player, map->tile_width, y, j);
                        SDL_UpdateWindowSurface(window);
                        SDL_Delay(16);
                    }
                    continue;
                    break;
                }
                break;

            default:
                break;
            }

            // This is used to determine which image to use.
            i++;
            if (i > 2)
                i = 0;
        }
        // Render the map and the character.
        renderMap(window, map);
        renderCharacter(window, player, map->tile_width, player->facing, i);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
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