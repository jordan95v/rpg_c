#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
typedef struct
{
    int x;
    int y;
    int health;
    char orientation[2];
} Entity;

void renderCharacter(SDL_Window *window, Entity *character, char *filename, int width, int height);
Entity *createEntity(int x, int y, int health);