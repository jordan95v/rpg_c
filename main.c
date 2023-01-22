#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Personnage sur carte", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *map = SDL_LoadBMP("assets/SNES_-_The_Legend_of_Zelda_A_Link_to_the_Past_-_Light_World.bmp");
    SDL_Texture *map_texture = SDL_CreateTextureFromSurface(renderer, map);

    SDL_Surface *character = SDL_LoadBMP("assets/character.bmp");
    SDL_Texture *character_texture = SDL_CreateTextureFromSurface(renderer, character);

    int x = 0, y = 0;
    int speed = 5;

    while (1)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    y -= speed;
                    break;
                case SDLK_DOWN:
                    y += speed;
                    break;
                case SDLK_LEFT:
                    x -= speed;
                    break;
                case SDLK_RIGHT:
                    x += speed;
                    break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map_texture, NULL, NULL);
        SDL_RenderCopy(renderer, character_texture, NULL, &(SDL_Rect){x, y, character->w, character->h});
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(map_texture);
    SDL_FreeSurface(map);
    SDL_DestroyTexture(character_texture);
    SDL_FreeSurface(character);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}