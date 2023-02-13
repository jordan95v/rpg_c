#include <SDL2/SDL.h>

typedef struct
{
    int **schema;
} Layer;

typedef struct
{
    int tile_width;
    int tile_height;
    int x_tiles;
    int y_tiles;
    SDL_Surface *tileset;
    SDL_Rect *tiles;
    Layer *layers;
    int number_of_layer;
    int width_map;
    int height_map;
    int dead_zone_number;
} Map;

Map *loadMap(const char *level);
void renderMap(SDL_Surface *window, Map *map);
int freeMap(Map *map);
int checkMove(Map *map, int x, int y);
void randMap(Map *map);
void reloadMap(Map *map, char *filename);