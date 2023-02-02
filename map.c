#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "functions.h"

#define CACHE_SIZE 5000

void loadTileset(FILE *file, Map *map)
{
    int numtile, i, j, width, height;
    char buffer[CACHE_SIZE], secondBuffer[CACHE_SIZE];

    fscanf(file, "%s", buffer);
    map->tileset = loadImage(buffer);

    fscanf(file, "%d %d", &map->x_tiles, &map->y_tiles);
    map->tile_width = map->tileset->w / map->x_tiles;
    map->tile_height = map->tileset->h / map->y_tiles;
    map->tiles = malloc(map->x_tiles * map->y_tiles * sizeof(Tile));

    for (j = 0, numtile = 0; j < map->y_tiles; j++)
    {
        for (i = 0; i < map->x_tiles; i++, numtile++)
        {
            map->tiles[numtile].rect.w = map->tile_width;
            map->tiles[numtile].rect.h = map->tile_height;
            map->tiles[numtile].rect.x = i * map->tile_width;
            map->tiles[numtile].rect.y = j * map->tile_height;

            fscanf(file, "%s %s", buffer, secondBuffer);
            map->tiles[numtile].full = 0;
            if (strcmp(secondBuffer, "full") == 0)
                map->tiles[numtile].full = 1;
        }
    }
}

void loadMapLevel(FILE *file, Map *map)
{
    int i, j, tmp;
    fscanf(file, "%d %d", &map->width_map, &map->height_map);
    map->schema = malloc(map->width_map * sizeof(char *));

    for (i = 0; i < map->width_map; i++)
        map->schema[i] = malloc(map->height_map * sizeof(char));

    for (j = 0; j < map->height_map; j++)
    {
        for (i = 0; i < map->width_map; i++)
        {
            fscanf(file, "%d", &tmp);
            if (tmp >= map->x_tiles * map->y_tiles)
                raise("Tile out of map. Check your map array.");
            map->schema[i][j] = tmp;
        }
    }
}

Map *loadMap(const char *level)
{
    FILE *file;
    Map *map;
    char buffer[CACHE_SIZE];

    file = fopen(level, "r");
    if (!file)
        raise("Cannot find the file.");

    map = malloc(sizeof(Map));

    do
    {
        fgets(buffer, CACHE_SIZE, file);
        if (strcmp(buffer, "#tileset\n") == 0)
            loadTileset(file, map);
        if (strcmp(buffer, "#level\n") == 0)
            loadMapLevel(file, map);
    } while (strcmp(buffer, "#fin") != 0);
    fclose(file);

    return map;
}

void renderMap(SDL_Window *window, Map *map)
{
    int i, j, tile_number;
    SDL_Rect dest;

    for (i = 0; i < map->width_map; i++)
    {
        for (j = 0; j < map->height_map; j++)
        {
            dest.x = i * map->tile_width;
            dest.y = j * map->tile_height;
            tile_number = map->schema[i][j];
            SDL_BlitSurface(map->tileset, &(map->tiles[tile_number].rect), SDL_GetWindowSurface(window), &dest);
        }
    }
}

int freeMap(Map *map)
{
    int i;
    SDL_FreeSurface(map->tileset);
    for (i = 0; i < map->height_map; i++)
        free(map->schema[i]);
    free(map->schema);
    free(map->tiles);
    free(map);
    return 0;
}