#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
typedef struct
{
    float x;
    float y;
    int health;
    SDL_Surface *tileset;
    int facing;
} Entity;

void renderCharacter(SDL_Window *window, Entity *character, int size, int y, int shift, char *mode);
Entity *createEntity(int x, int y, int health, char *filename);
void freePlayer(Entity *character);