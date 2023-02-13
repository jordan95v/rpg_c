#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void raise(char *error)
{
    printf("[ERROR] %s\n", error);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

SDL_Surface *loadImage(char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)
        raise("Couldn't loading the tileset, check the filename.");
    return surface;
}

int showMenu(SDL_Window *window)
{
    SDL_Rect menu = {370, 250, 200, 50};
    SDL_Surface *backgroundMenu = loadImage("assets/menu.png");
    SDL_Surface *backgroundBtn = loadImage("assets/button.png");

    SDL_Event event;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return -1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= menu.x && x <= menu.x + menu.w && y >= menu.y && y <= menu.y + menu.h)
                {
                    SDL_FreeSurface(backgroundMenu);
                    SDL_FreeSurface(backgroundBtn);
                    return 0;
                }
            }
        }
        SDL_BlitSurface(backgroundMenu, NULL, SDL_GetWindowSurface(window), NULL);
        SDL_BlitSurface(backgroundBtn, NULL, SDL_GetWindowSurface(window), &menu);
        SDL_UpdateWindowSurface(window);
    }
}

int init(SDL_Window **window, int w, int h)
{
    // Init the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    // Init the SDL_Image PNG.
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        raise("Error initializing sdl-image.");

    // Init the TTF
    if (TTF_Init() < 0)
        raise("Error initializing sdl-ttf");

    // Create the window.
    *window = SDL_CreateWindow("Zelda Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    return 0;
}

void renderCoinsUI(SDL_Surface *font, SDL_Surface *window)
{
    SDL_Rect text_rect;
    text_rect.x = window->w - font->w - 10;
    text_rect.y = 10;
    SDL_BlitSurface(font, NULL, window, &text_rect);
}

SDL_Surface *createCoinsFont(char *filename, int coins)
{
    TTF_Font *font = TTF_OpenFont(filename, 24);
    SDL_Color color = {255, 255, 255};
    char text[20];

    if (!font)
        raise("Error initializing font.");

    sprintf(text, "%d coins", coins);
    return TTF_RenderText_Solid(font, text, color);
}