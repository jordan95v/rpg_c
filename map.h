#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    SDL_Rect rect;
    int full;
} Tile;

typedef struct
{
    int tile_width;
    int tile_height;
    int x_tiles;
    int y_tiles;
    SDL_Surface *tileset;
    Tile *tiles;
    unsigned char **schema;
    int width_map;
    int height_map;
} Map;

Map *loadMap(const char *level);
void renderMap(SDL_Window *window, Map *map);
int freeMap(Map *map);