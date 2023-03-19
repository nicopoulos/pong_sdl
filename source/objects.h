#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdint.h>
#include <SDL2/SDL.h>


// Paddle / "Schläger"
typedef struct paddle_t
{
    size_t height;
    size_t width;
    float x;
    float y;

} paddle_t;


void render_paddle(paddle_t* paddle, SDL_Renderer* renderer, float unit)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Weisses Paddle
    SDL_Rect paddle_rect = {.h = paddle->height * unit, .w = paddle->width * unit, .x = paddle->x * unit, .y = paddle->y * unit};
    SDL_RenderFillRect(renderer, &paddle_rect);
}

typedef struct ball_t
{
    size_t size; 
    float x;
    float y;
    float velocity_x;
    float velocity_y;

} ball_t;

void render_ball(ball_t* ball, SDL_Renderer* renderer, float unit)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Weisser Ball
    SDL_Rect ball_rect = {.h = ball->size * unit, .w = ball->size * unit, .x = ball->x * unit, .y = ball->y * unit};
    SDL_RenderFillRect(renderer, &ball_rect);
}





#endif