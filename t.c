#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Rect menu_block1;
    SDL_Surface *background_surface;
    SDL_Texture *background_texture;
    SDL_Surface *button_surface;
    SDL_Texture *button_texture;

    menu_block1.x = 370;
    menu_block1.y = 250;
    menu_block1.w = 200;
    menu_block1.h = 50;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Menu Block", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    background_surface = SDL_LoadBMP("menu.bmp");
    background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);

    button_surface = SDL_LoadBMP("button.bmp");
    button_texture = SDL_CreateTextureFromSurface(renderer, button_surface);
    SDL_FreeSurface(button_surface);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                goto quit;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= menu_block1.x && x <= menu_block1.x + menu_block1.w && y >= menu_block1.y && y <= menu_block1.y + menu_block1.h)
                {
                    printf("Bouton1\n");
                }
            }
        }
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_RenderCopy(renderer, button_texture, NULL, &menu_block1);
        SDL_RenderPresent(renderer);
    }

quit:
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
