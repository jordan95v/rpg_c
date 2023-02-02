#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "player.h"

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
    Entity *player = createEntity(10, 7, 100);

    // Window creation.
    SDL_Window *window = NULL;
    if (init(&window, map->width_map * map->tile_width, map->height_map * map->tile_height) != 0)
        goto Quit;

    char image[100] = "assets/character/front1.png";
    int i = 1;

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
                    sprintf(image, "assets/character/back%d.png", i);
                    break;
                case SDLK_DOWN:
                    if (player->y < map->height_map - 2)
                        player->y++;
                    sprintf(image, "assets/character/front%d.png", i);
                    break;
                case SDLK_RIGHT:
                    if (player->x < map->width_map - 1)
                        player->x++;
                    sprintf(image, "assets/character/right%d.png", i);
                    break;
                case SDLK_LEFT:
                    if (player->x > 0)
                        player->x--;
                    sprintf(image, "assets/character/left%d.png", i);
                    break;
                }
                break;

            default:
                break;
            }
            i++;
            if (i > 3)
                i = 1;
        }

        // Render the map and the character.
        renderMap(window, map);
        renderCharacter(window, player, image, map->tile_width, map->tile_height);
        SDL_UpdateWindowSurface(window);
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