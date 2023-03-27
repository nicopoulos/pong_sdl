#include <stdbool.h>

#include "home.h"
#include "elements.h"
#include "game.h"


// constants

// external variables
extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern int window_width;
extern int window_height;

// local variables
bool quit_home = false;
SDL_Texture* background = NULL;
TTF_Font* home_font;

button_t start_button;
button_t quit_button; 

int start_game()
{
    game();
    return 0;
}

int home_input()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
        {
            printf("Quit\n");
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
                    start_button.selected = true;
                    quit_button.selected = false;
                    break;
                }
                case SDL_SCANCODE_DOWN:
                {
                    start_button.selected = false;
                    quit_button.selected = true;
                    break;
                }
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_SPACE:
                {
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

    }

    return 0;
}

int update_home()
{
    return 0;
}

int render_home()
{
    // background
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    // buttons
    render_button(renderer, &start_button);
    render_button(renderer, &quit_button);


    SDL_RenderPresent(renderer);

    return 0;
}


int home()
{
    printf("Home\n");
    SDL_Surface* temp = IMG_Load("assets/home_background.jpg");
    background = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);

    home_font = TTF_OpenFont("assets/prstart.ttf", 30); 

    float padding = window_height / 15.0;

    start_button.rect.h = window_height / 6.0;
    start_button.rect.w = window_width / 3.0;
    start_button.rect.x = (window_width - start_button.rect.w) / 2.0;
    start_button.rect.y = (window_height - start_button.rect.h) / 2.0 - padding;
    start_button.font = home_font;
    start_button.selected = true;
    snprintf(start_button.text, 20, "Spiel Starten");

    quit_button.rect = start_button.rect;
    quit_button.rect.y = window_height / 2.0 + padding;
    quit_button.font = home_font;
    quit_button.selected = false;
    snprintf(quit_button.text, 20, "Beenden");

    while(!quit_home)
    {
        // Rendering
        render_home();
        home_input();
    }
    TTF_CloseFont(home_font);
    SDL_DestroyTexture(background);
    return 0;
}




