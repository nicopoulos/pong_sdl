#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

#include "home.h"
#include "elements.h"
#include "game.h"
#include "controller_handler.h"

// constants
#define JOYSTICK_DEADZONE 8000

// external variables
extern SDL_Window* screen;
extern SDL_Renderer* renderer;

extern player_t left_player;
extern player_t right_player;

extern Mix_Chunk* select_button_sound;

extern int window_width;
extern int window_height;

extern TTF_Font* main_font;

// local variables
bool quit_home = false;
SDL_Texture* background;
SDL_Texture* title_texture;
SDL_Rect title_rect;

button_t start_button;
button_t quit_button; 

int start_game()
{
    while(game());
    return 0;
}

bool home_input()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    bool rerender_necessary = true;
    switch(event.type)
    {
        case SDL_CONTROLLERAXISMOTION:
            rerender_necessary = false;
            break;
        case SDL_QUIT:
        {
            quit_home = true;
            break;
        }
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                {
                    quit_home = true;
                    break;
                }
                case SDL_SCANCODE_UP:
                {
                    if (quit_button.selected)
                    {
                        start_button.selected = true;
                        quit_button.selected = false;
                        Mix_PlayChannel(-1, select_button_sound, 0);
                    }
                    break;
                }
                case SDL_SCANCODE_DOWN:
                {
                    if (start_button.selected)
                    {
                        start_button.selected = false;
                        quit_button.selected = true;
                        Mix_PlayChannel(-1, select_button_sound, 0);
                    }
                    break;
                }
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_SPACE:
                {
                    Mix_PlayChannel(-1, select_button_sound, 0);

                    if (start_button.selected)
                        start_game();
                    else if (quit_button.selected)
                        quit_home = true;
                    break;
                }
                default:
                    break;
            }
            break;
        }



        case SDL_CONTROLLERBUTTONDOWN:
        {
            if (SDL_GameControllerFromInstanceID(event.cbutton.which) == left_player.gamepad)
            {
                switch(event.cbutton.button)
                {
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    {
                        if (quit_button.selected)
                        {
                            start_button.selected = true;
                            quit_button.selected = false;
                            Mix_PlayChannel(-1, select_button_sound, 0);
                        }
                        break;
                    } 
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    {
                        if (start_button.selected)
                        {
                            quit_button.selected = true;
                            start_button.selected = false;
                            Mix_PlayChannel(-1, select_button_sound, 0);
                        }
                        break;
                    }
                    case SDL_CONTROLLER_BUTTON_B:
                    case SDL_CONTROLLER_BUTTON_A:
                    case SDL_CONTROLLER_BUTTON_X:
                    case SDL_CONTROLLER_BUTTON_Y:
                    {
                        Mix_PlayChannel(-1, select_button_sound, 0);
                        if (start_button.selected)
                            start_game();
                        else if (quit_button.selected)
                            quit_home = true;
                        break;
                    }
                    default:
                        break;
                }
            }
    
            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED:
        {
            on_controller_removed(event.cdevice.which);

            break;
        }
        case SDL_CONTROLLERDEVICEADDED:
        {
            on_controller_added(event.cdevice.which);

            break;
        }
    }

    return rerender_necessary;
}

int render_home()
{
    // background
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    // title
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    // buttons
    render_button(renderer, &start_button);
    render_button(renderer, &quit_button);


    SDL_RenderPresent(renderer);

    return 0;
}


int home()
{
    SDL_Surface* temp = IMG_Load("assets/home_background.jpg");
    background = SDL_CreateTextureFromSurface(renderer, temp);

    main_font = TTF_OpenFont("assets/prstart.ttf", 30); 


    // title 

    float padding = window_height / 20.0;
    temp = TTF_RenderText_Solid(main_font, "TENNIS", (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
    title_texture = SDL_CreateTextureFromSurface(renderer, temp);
    title_rect.h = window_height / 4.0;
    title_rect.w = window_width / 2.0;
    title_rect.y = padding;
    title_rect.x = window_width / 4.0;

    SDL_FreeSurface(temp);

    // buttons
    start_button.rect.h = window_height / 5.0;
    start_button.rect.w = window_width / 2.5;
    start_button.rect.x = (window_width - start_button.rect.w) / 2.0;
    start_button.rect.y = (window_height - start_button.rect.h) / 2.0;
    start_button.font = main_font;
    start_button.margin = start_button.rect.h / 8.0;
    start_button.selected = true;
    snprintf(start_button.text, 20, "Spiel Starten");

    quit_button.rect = start_button.rect;
    quit_button.rect.y = start_button.rect.y + start_button.rect.h + padding;
    quit_button.font = start_button.font;
    quit_button.margin = start_button.margin;
    quit_button.selected = false;
    snprintf(quit_button.text, 20, "Beenden");

    render_home();
    while(!quit_home)
    {
        if (home_input())
            render_home();
    }

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(title_texture);

    return 0;
}




