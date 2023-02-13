#include <SDL2/SDL_ttf.h>

void raise(char *error);
SDL_Surface *loadImage(char *filename);
int init(SDL_Window **window, int w, int h);
int showMenu(SDL_Window *window);
SDL_Surface *createFont(char *filename, char *format, int value);
void renderCoinsUI(TTF_Font *font, SDL_Surface *window, int value);
void renderHeartUI(TTF_Font *font, SDL_Surface *heart, SDL_Surface *window, int value);
