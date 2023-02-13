void raise(char *error);
SDL_Surface *loadImage(char *filename);
int init(SDL_Window **window, int w, int h);
int showMenu(SDL_Window *window);
SDL_Surface *createCoinsFont(char *filename, int coins);
void renderCoinsUI(SDL_Surface *font, SDL_Surface *window);