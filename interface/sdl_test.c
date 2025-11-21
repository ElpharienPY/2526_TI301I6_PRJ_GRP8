#include <stdio.h>

// Tell SDL not to redefine main() as SDL_main()
#define SDL_MAIN_HANDLED
#include <SDL.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
            "SDL Test",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480,
            SDL_WINDOW_SHOWN
    );

    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Delay(2000);  // keep the window visible for 2 seconds

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
