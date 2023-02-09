#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void loadLayer(FILE *file, Map *map, int index, char *mode)
{
    int i, j, tmp;

    if (strstr(mode, "new"))
    {
        printf("hello\n");
        map->layers[index].schema = (int **)malloc(map->width_map * sizeof(int *));

        for (i = 0; i < map->width_map; i++)
            map->layers[index].schema[i] = (int *)malloc(map->height_map * sizeof(int));
    }

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

    // Gets the width and the height of the map.
    fgets(buffer, CACHE_SIZE, file);
    sscanf(buffer, "%d %d", &map->width_map, &map->height_map);

    // Gets the dead zone tile number.
    fgets(buffer, CACHE_SIZE, file);
    sscanf(buffer, "%d", &map->dead_zone_number);

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
            map->number_of_layer++;
            loadLayer(map_file, map, i, "new");
            i++;
        }
        fclose(map_file);
    }
}

void reloadMap(Map *map, char *filename)
{
    FILE *file = fopen(filename, "r");
    FILE *map_file = NULL;
    int i = 0;
    char buffer[CACHE_SIZE];

    if (!file)
        raise("Error reading file.");

    while (fgets(buffer, CACHE_SIZE, file) != NULL)
    {
        // Remove the \n after the file name, weird cause it work with the tileset :/
        strcpy(buffer, strtok(buffer, "\n"));

        map_file = fopen(buffer, "r");
        if (!map_file)
            break;
        else
        {
            loadLayer(map_file, map, i, "reload");
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
    for (int i = 0; i < map->number_of_layer; i++)
    {
        for (int j = 0; j < map->y_tiles; j++)
        {
            free(map->layers[i].schema[j]);
        }
        free(map->layers[i].schema);
    }
    free(map->layers);
    free(map->tiles);
    SDL_FreeSurface(map->tileset);
    free(map);
    return 0;
}

int checkMove(Map *map, int x, int y)
{
    // printf("%d\n", map->layers[2].schema[x][y]);
    if (map->layers[0].schema[x][y] == map->dead_zone_number && map->layers[1].schema[x][y] < 60)
        return 1;
    return 0;
}

void setObject(Map *map, int i, int j, int first, int second)
{
    map->layers[3].schema[i][j] = first;
    map->layers[0].schema[i][j] = -1;

    map->layers[3].schema[i][j + 1] = second;
    map->layers[0].schema[i][j + 1] = -1;
}

void randRock(Map *map)
{
    srand(time(0));
    for (int i = 6; i < map->width_map; i++)
    {
        for (int j = 0; j < map->height_map; j++)
        {
            if (map->layers[0].schema[i][j] == map->dead_zone_number && map->layers[2].schema[i][j + 1] == -1)
            {
                int buff;
                buff = rand() % 111;
                map->layers[0].schema[i][j] = -1;

                // What a big if - else if, no idea of how i could refactor this.
                if (buff == 1)
                    map->layers[3].schema[i][j] = 576;
                else if (buff == 10)
                    map->layers[3].schema[i][j] = 577;
                else if (buff == 20)
                    map->layers[3].schema[i][j] = 578;
                else if (buff == 30)
                    map->layers[3].schema[i][j] = 579;
                else if (buff == 40)
                    map->layers[3].schema[i][j] = 580;
                else if (buff == 50)
                    map->layers[3].schema[i][j] = 581;
                else if (buff == 60)
                    map->layers[3].schema[i][j] = 514;
                else if (buff == 70)
                    map->layers[3].schema[i][j] = 337;
                else if (buff == 80)
                    map->layers[3].schema[i][j] = 339;
                else if (buff == 90)
                    setObject(map, i, j, 466, 482);
                else if (buff == 100)
                    setObject(map, i, j, 530, 546);
                else if (buff == 110)
                    setObject(map, i, j, 434, 450);
                else
                    map->layers[0].schema[i][j] = map->dead_zone_number;
            }
        }
    }
}

void randFloor(Map *map)
{
    srand(time(0));
    for (int i = 0; i < map->width_map; i++)
    {
        for (int j = 0; j < map->height_map; j++)
        {
            if (map->layers[1].schema[i][j] == -1)
                continue;

            int buff;
            buff = rand() % 151;

            // What a big if - else if, no idea of how i could refactor this.
            if (buff == 1)
                map->layers[1].schema[i][j] = 4;
            else if (buff == 10)
                map->layers[1].schema[i][j] = 5;
            else if (buff == 20)
                map->layers[1].schema[i][j] = 6;
            else if (buff == 30)
                map->layers[1].schema[i][j] = 7;
            else if (buff == 40)
                map->layers[1].schema[i][j] = 20;
            else if (buff == 50)
                map->layers[1].schema[i][j] = 21;
            else if (buff == 60)
                map->layers[1].schema[i][j] = 22;
            else if (buff == 70)
                map->layers[1].schema[i][j] = 23;
            else if (buff == 80)
                map->layers[1].schema[i][j] = 36;
            else if (buff == 90)
                map->layers[1].schema[i][j] = 37;
            else if (buff == 100)
                map->layers[1].schema[i][j] = 38;
            else if (buff == 110)
                map->layers[1].schema[i][j] = 39;
            else if (buff == 120)
                map->layers[1].schema[i][j] = 52;
            else if (buff == 130)
                map->layers[1].schema[i][j] = 53;
            else if (buff == 140)
                map->layers[1].schema[i][j] = 54;
            else if (buff == 150)
                map->layers[1].schema[i][j] = 55;
        }
    }
}

void setTomb(Map *map, int i, int j, int first, int second, int third)
{
    map->layers[3].schema[i][j] = first;
    map->layers[0].schema[i][j] = -1;

    map->layers[3].schema[i][j + 1] = second;
    map->layers[0].schema[i][j + 1] = -1;

    map->layers[3].schema[i + 1][j + 1] = third;
}

void randTomb(Map *map)
{
    srand(time(0));
    for (int i = 6; i < map->width_map; i++)
    {
        for (int j = 0; j < map->height_map; j++)
        {
            if (map->layers[0].schema[i][j] == map->dead_zone_number && map->layers[2].schema[i][j + 1] == -1)
            {
                int buff;
                buff = rand() % 150;

                if (buff == 1)
                    setTomb(map, i, j, 516, 532, 533);
                else if (buff == 10)
                    setTomb(map, i, j, 484, 500, 501);
                else if (buff == 20)
                    setTomb(map, i, j, 456, 472, 473);
            }
        }
    }
}

void randMap(Map *map)
{
    randRock(map);
    randFloor(map);
    randTomb(map);
}