#include "elements.h"
#include "constants.h"

#define MARGIN 40





void render_button(SDL_Renderer* renderer, const button_t* button)
{
    if (button->selected == true)
    {
        SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0, 0xFF); // Gelb
        SDL_Rect select_rect = {.h = button->rect.h + MARGIN, .w = button->rect.w + MARGIN, .x = button->rect.x - (MARGIN / 2.0), .y = button->rect.y - (MARGIN / 2.0)};
        SDL_RenderFillRect(renderer, &select_rect);
    }

    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_RenderFillRect(renderer, &(button->rect));

    // text
    SDL_Surface* surf = TTF_RenderText_Solid(button->font, button->text, (SDL_Colour){0xFF, 0xFF, 0xFF, 0xFF});
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect text_rect = {.h = button->rect.h - MARGIN, .w = button->rect.w - MARGIN, .x = button->rect.x + (MARGIN / 2.0), .y = button->rect.y + (MARGIN / 2.0)};
    SDL_RenderCopy(renderer, text, NULL, &text_rect);
}


// for game objects

void render_paddle(paddle_t* paddle, SDL_Renderer* renderer, double unit)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Weisses Paddle
    SDL_Rect paddle_rect = {.h = paddle->height * unit, .w = paddle->width * unit, .x = paddle->pos_x * unit, .y = paddle->pos_y * unit};
    SDL_RenderFillRect(renderer, &paddle_rect);
}


void render_ball(ball_t* ball, SDL_Renderer* renderer, double unit)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Weisser Ball
    SDL_Rect ball_rect = {.h = ball->size * unit, .w = ball->size * unit, .x = ball->pos_x * unit, .y = ball->pos_y * unit};
    SDL_RenderFillRect(renderer, &ball_rect);
}

void set_ball_direction(ball_t* ball, double angle)
{
    ball->velocity_x = cos(angle);
    ball->velocity_y = sin(angle);
}




