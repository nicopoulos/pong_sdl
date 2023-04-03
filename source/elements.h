#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>




// UI

typedef struct button_t
{
    SDL_Rect rect;
    TTF_Font* font;
    char text[20];
    unsigned int margin;
    bool selected;

} button_t;



// Game Objects


typedef struct score_t
{
    size_t count;
    size_t length;
    char string[10];

} score_t;

typedef struct paddle_t
{
    size_t height;
    size_t width;
    double pos_x;
    double pos_y;
    double movement;


} paddle_t;

typedef struct player_t
{
    paddle_t paddle;
    score_t score;
    SDL_GameController* gamepad;

} player_t;


typedef struct ball_t
{
    size_t size; 
    double pos_x;
    double pos_y;
    double velocity_x;
    double velocity_y;

} ball_t;




void render_button(SDL_Renderer* renderer, const button_t* button);

void render_paddle(paddle_t* paddle, SDL_Renderer* renderer, double unit);

void render_ball(ball_t* ball, SDL_Renderer* renderer, double unit);

void set_ball_direction(ball_t* ball, double angle);

#endif