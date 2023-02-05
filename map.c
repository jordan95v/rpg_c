#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "functions.h"

#define CACHE_SIZE 5000

void loadTileset(FILE *file, Map *map)
{
    int numtile, i, j, width, height;
    char buffer[CACHE_SIZE];

    fscanf(file, "%s", buffer);
    map->tileset = loadImage(buffer);

    fscanf(file, "%d %d", &map->x_tiles, &map->y_tiles);
    map->tile_width = map->tileset->w / map->x_tiles;
    map->tile_height = map->tileset->h / map->y_tiles;
    map->tiles = malloc(map->x_tiles * map->y_tiles * sizeof(SDL_Rect));

    for (j = 0, numtile = 0; j < map->y_tiles; j++)
    {
        for (i = 0; i < map->x_tiles; i++, numtile++)
        {
            map->tiles[numtile].w = map->tile_width;
            map->tiles[numtile].h = map->tile_height;
            map->tiles[numtile].x = i * map->tile_width;
            map->tiles[numtile].y = j * map->tile_height;
        }
    }
}

void loadLayer(FILE *file, Map *map, int index)
{
    int i, j, tmp;
    map->number_of_layer++;
    map->layers[index].schema = malloc(map->width_map * sizeof(int *));

    for (i = 0; i < map->width_map; i++)
        map->layers[index].schema[i] = malloc(map->height_map * sizeof(int));

    for (j = 0; j < map->height_map; j++)
    {
        for (i = 0; i < map->width_map; i++)
        {
            fscanf(file, "%d", &tmp);
            if (tmp >= map->x_tiles * map->y_tiles)
                raise("Tile out of map. Check your map array.");
            map->layers[index].schema[i][j] = tmp;
        }
    }
}

void loadMapLevel(FILE *file, Map *map)
{
    int i = 0;
    char buffer[CACHE_SIZE];
    FILE *map_file;

    fgets(buffer, CACHE_SIZE, file);
    sscanf(buffer, "%d %d", &map->width_map, &map->height_map);

    while (fgets(buffer, CACHE_SIZE, file) != NULL)
    {
        // Remove the \n after the file name, weird cause it work with the tileset :/
        strcpy(buffer, strtok(buffer, "\n"));

        map_file = fopen(buffer, "r");
        if (!map_file)
            break;
        else
        {
            map->layers = (Layer *)realloc(map->layers, sizeof(Layer) * i + 1);
            loadLayer(map_file, map, i);
            i++;
        }
        fclose(map_file);
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
    map->number_of_layer = 0;

    while (fgets(buffer, CACHE_SIZE, file) != NULL)
    {
        if (strstr(buffer, "#tileset"))
            loadTileset(file, map);
        else if (strstr(buffer, "#level"))
            loadMapLevel(file, map);
    }
    fclose(file);

    return map;
}

void renderMap(SDL_Window *window, Map *map)
{
    int i, j, layer_number, tile_number;
    SDL_Rect dest;

    for (layer_number = 0; layer_number < map->number_of_layer; layer_number++)
    {
        for (i = 0; i < map->width_map; i++)
        {
            for (j = 0; j < map->height_map; j++)
            {
                dest.x = i * map->tile_width;
                dest.y = j * map->tile_height;
                tile_number = map->layers[layer_number].schema[i][j];
                SDL_BlitSurface(map->tileset, &(map->tiles[tile_number]), SDL_GetWindowSurface(window), &dest);
            }
        }
    }
}

int freeMap(Map *map)
{
    int i, j;
    SDL_FreeSurface(map->tileset);
    for (i = 0; i < map->number_of_layer; i++)
    {
        for (j = 0; j < map->height_map; j++)
        {
            free(map->layers[i].schema[j]);
        }
    }
    free(map->layers);
    free(map->tiles);
    free(map);
    return 0;
}

int checkMove(Map *map, int x, int y)
{
    int exclude[6] = {308, 306, 289, 290, 288, 346};
    if (map->layers[0].schema[x][y] == 72)
    {
        for (int i = 0; i < 6; i++)
        {
            if (map->layers[2].schema[x][y] == exclude[i])
                return 0;
        }

        return 1;
    }
    return 0;
}