#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"

typedef struct
{
    int x;
    int y;
    int health;
    SDL_Surface *tileset;
    int facing;
    int will_attack;
} Entity;

void renderCharacter(SDL_Window *window, Entity *character, int size, int shift, char *mode);
Entity *createEntity(int x, int y, int health, char *filename);
Entity *createEnemy(int x, int y, int health, SDL_Surface *surface);
void moveEnemy(Map *map, Entity **enemies, int enemies_number, Entity *player);
int checkAttack(Entity *attacker, Entity *victim);
void attack(Entity *attacker, Entity *victim);
int checkCollision(Entity *player, Entity **enemies, int enemies_number);
void freePlayer(Entity *character);