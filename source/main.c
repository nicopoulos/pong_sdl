#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "home.h"
#include "game.h"

SDL_Window* screen;
SDL_Renderer* renderer;
SDL_AudioDeviceID* audio_dev_id;

int window_width;
int window_height;

double unit;

TTF_Font* main_font;

int main()
{
    // Setup
    // SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    screen = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 10, 10, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(screen, &window_width, &window_height);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // SDL_image
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    // SDL_ttf
    TTF_Init();


    unit = window_width / 50.0;

    main_font = TTF_OpenFont("assets/prstart.ttf", 25);


    // Menu
    home();

    // Game loop


    // Clean up
    TTF_CloseFont(main_font);

    TTF_Quit();

    IMG_Quit();

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(screen);

    SDL_Quit();

    return 0;
}


