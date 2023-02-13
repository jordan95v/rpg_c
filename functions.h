void raise(char *error);
SDL_Surface *loadImage(char *filename);
int init(SDL_Window **window, int w, int h);
int showMenu(SDL_Window *window);
SDL_Surface *createFont(char *filename, char *format, int value);
void renderCoinsUI(SDL_Surface *font, SDL_Surface *window);
void renderHeartUI(SDL_Surface *font, SDL_Surface *heart, SDL_Surface *window);
