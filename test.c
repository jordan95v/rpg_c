#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "functions.h"

int main()
{
    Map *map = loadMap("maps/main/map.txt");
    printf("%d\n", map->tile_width);
    printf("%d", map->height_map);

    for (int j = 0; j < map->height_map; j++)
    {
        for (int i = 0; i < map->width_map; i++)
        {

            printf("%d ", map->layers[200000].schema[i][j]);
        }
        printf("\n");
    }
}