#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "constants.h"
#include "elements.h"
#include "home.h"
#include "game.h"

SDL_Window* screen;
SDL_Renderer* renderer;

Mix_Chunk* hit_paddle_sound;
Mix_Chunk* hit_wall_sound;
Mix_Chunk* applause_sound;

SDL_GameController* left_gamepad = NULL;
SDL_GameController* right_gamepad = NULL;

// players
player_t left_player;
player_t right_player;

int window_width;
int window_height;

double unit;

TTF_Font* main_font;

int main()
{
    // Setup
    // SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    screen = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 10, 10, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (screen == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }
    SDL_GetWindowSize(screen, &window_width, &window_height);
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    // SDL_image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == false)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }
    // SDL_ttf
    if (TTF_Init() != 0)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }
    // SDL_mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    hit_paddle_sound = Mix_LoadWAV("assets/paddle_sound.wav");
    hit_wall_sound = Mix_LoadWAV("assets/wall_sound.wav");
    applause_sound = Mix_LoadWAV("assets/applause.wav");


    main_font = TTF_OpenFont("assets/prstart.ttf", 25);
    if (main_font == NULL)
    {
        printf("%s\n", SDL_GetError());
        return 1;
    }

    unit = window_width / 50.0;


    // initialize controllers
    left_player.gamepad = NULL;
    right_player.gamepad = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            if (left_player.gamepad == NULL)
            {
                left_player.gamepad = SDL_GameControllerOpen(i);
            }
            else if (right_player.gamepad == NULL)
            {
                right_player.gamepad = SDL_GameControllerOpen(i);
            }
        }
    }


    // Menu
    home();

    // Game loop


    // Clean up

    SDL_GameControllerClose(left_gamepad);
    left_gamepad = NULL;
    SDL_GameControllerClose(right_gamepad);
    right_gamepad = NULL;


    Mix_FreeChunk(hit_wall_sound);
    Mix_FreeChunk(hit_paddle_sound);
    Mix_FreeChunk(applause_sound);

    Mix_CloseAudio();

    TTF_CloseFont(main_font);

    TTF_Quit();

    IMG_Quit();

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(screen);

    SDL_Quit();

    return 0;
}


