#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
typedef struct
{
    int x;
    int y;
    int health;
    SDL_Surface *tileset;
    int facing;
} Entity;

void renderCharacter(SDL_Window *window, Entity *character, int size, int shift, char *mode);
Entity *createEntity(int x, int y, int health, char *filename);
void freePlayer(Entity *character);