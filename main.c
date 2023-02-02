#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "player.h"
#include "functions.h"

#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

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
    Entity *player = createEntity(10, 7, 100, "assets/char2.png");

    // Window creation.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;

    int i = 0;

    // Main loop.
    while (1)
    {
        // Event management
        SDL_Event event;
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
                    if (player->y > 0)
                        player->y--;
                    player->facing = UP;
                    break;

                case SDLK_DOWN:
                    if (player->y < map->height_map - 2)
                        player->y++;
                    player->facing = DOWN;
                    break;

                case SDLK_RIGHT:
                    if (player->x < map->width_map - 1)
                        player->x++;
                    player->facing = RIGHT;
                    break;

                case SDLK_LEFT:
                    if (player->x > 0)
                        player->x--;
                    player->facing = LEFT;
                    break;

                    //     case SDLK_SPACE:
                    //         switch (player->orientation[0])
                    //         {
                    //         case 'n':
                    //             strcpy(facing, "back");
                    //             break;

                    //         case 's':
                    //             strcpy(facing, "front");
                    //             break;

                    //         case 'r':
                    //             strcpy(facing, "right");
                    //             break;

                    //         case 'l':
                    //             strcpy(facing, "left");
                    //             break;

                    //         default:
                    //             break;
                    //         }

                    //         for (int j = 1; j < 4; j++)
                    //         {
                    //             sprintf(image, "assets/character/%s_attack%d.png", facing, j);
                    //             renderCharacter(window, player, image, map->tile_width, map->tile_height);
                    //             SDL_UpdateWindowSurface(window);
                    //             SDL_Delay(30);
                    //         }
                    //         sprintf(image, "assets/character/%s%d.png", facing, i);
                    //         renderCharacter(window, player, image, map->tile_width, map->tile_height);
                    //         continue;
                    //         break;
                    //     }
                    //     break;

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
            renderCharacter(window, player, map->tile_width, i);

            SDL_UpdateWindowSurface(window);
        }
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