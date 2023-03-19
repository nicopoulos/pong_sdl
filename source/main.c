#include <stdio.h>

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#include "constants.h"
#include "objects.h"


// Globale Variabeln & Objekte
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int window_width_px = INITIAL_WINDOW_WIDTH;
int window_height_px = INITIAL_WINDOW_HEIGHT;


float unit = (INITIAL_WINDOW_WIDTH / 50); // Längeneinheit in Pixeln

float window_width_units = 50.0;
float window_height_units = INITIAL_WINDOW_HEIGHT * 50.0 / INITIAL_WINDOW_WIDTH;

paddle_t left_paddle = {.height = PADDLE_HEIGHT, .width = PADDLE_WIDTH};
paddle_t right_paddle = {.height = PADDLE_HEIGHT, .width = PADDLE_WIDTH};
ball_t ball = {.size = BALL_SIZE};

bool left_paddle_up = 0;
bool left_paddle_down = 0;
bool right_paddle_up = 0;
bool right_paddle_down = 0;

size_t score_left_player = 0;
size_t score_right_player = 0;

bool match_is_ongoing = true;
bool game_is_ongoing = true;
bool game_running = true;


void quit()
{
    game_is_ongoing = false;
    match_is_ongoing = false;
}

void on_window_resize()
{
    SDL_GetWindowSize(window, &window_width_px, &window_height_px);

    unit = (float)(window_width_px / 50);

    window_width_units = window_width_px / unit;
    window_height_units = window_height_px / unit;

    printf("Fensterdimensionen: %dx%d Pixel\n", window_height_px, window_width_px);
}

void set_ball_direction(float angle)
{
    ball.velocity_x = cosf(angle);
    ball.velocity_y = sinf(angle);
}


void handle_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit();
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    on_window_resize();
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit();
                    break;
                }

                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                        left_paddle_up = true;
                        break;
                    case SDL_SCANCODE_S:
                        left_paddle_down = true;                    
                        break;
                    case SDL_SCANCODE_UP:
                        right_paddle_up = true;
                        break;
                    case SDL_SCANCODE_DOWN:
                        right_paddle_down = true;
                        break;

                    default:
                        break;

                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                        left_paddle_up = false;
                        break;
                    case SDL_SCANCODE_S:
                        left_paddle_down = false;                    
                        break;
                    case SDL_SCANCODE_UP:
                        right_paddle_up = false;
                        break;
                    case SDL_SCANCODE_DOWN:
                        right_paddle_down = false;
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }

    }
}

void update()
{
    static Uint32 last_frame = 0U;

    if (SDL_GetTicks64() - last_frame < MIN_FRAME_DURATION)
        SDL_Delay((last_frame + MIN_FRAME_DURATION) - SDL_GetTicks64());

    float frame_duration_s = (float)(SDL_GetTicks64() - last_frame) / 1000.0; // Wie lange dauert ein Frame
    last_frame = SDL_GetTicks64();

    // Ball
    ball.x += ball.velocity_x * BALL_VELOCITY_FACTOR * frame_duration_s;
    ball.y += ball.velocity_y * BALL_VELOCITY_FACTOR * frame_duration_s;

    // Paddles

    // Linkes Paddle nach oben
    if (left_paddle_up && !left_paddle_down)
        left_paddle.y -= PADDLE_VELOCITY * frame_duration_s;
    // Linkes Paddle nach unten
    if (left_paddle_down && !left_paddle_up)
        left_paddle.y += PADDLE_VELOCITY * frame_duration_s;

    // Rechtes Paddle nach oben
    if (right_paddle_up && !right_paddle_down)
        right_paddle.y -= PADDLE_VELOCITY * frame_duration_s;
    // Rechtes Paddle nach unten
    if (right_paddle_down && !right_paddle_up)
        right_paddle.y += PADDLE_VELOCITY * frame_duration_s;

    // Damit die Paddle nicht aus dem Bildschirm gehen
    if (left_paddle.y <= 0)
        left_paddle.y = 0;
    else if (left_paddle.y + left_paddle.height >= window_height_units)
        left_paddle.y = window_height_units - left_paddle.height;
    
    if (right_paddle.y <= 0)
        right_paddle.y = 0;
    else if (right_paddle.y + right_paddle.height >= window_height_units)
        right_paddle.y = window_height_units - right_paddle.height;


    // Kollisionsabfragen

    // Ball mit oberer Wand
    if (ball.y <= 0)
    {
        ball.velocity_y *= -1;
        ball.y = 0;
    }
    // Ball mit unterer Wand
    else if ( ball.y + ball.size >= window_height_units)
    {
        ball.velocity_y *= -1;
        ball.y = window_height_units - ball.size;
    }

    
    float tolerance = (float)ball.size; // Wie tief darf der Ball im Paddle drin sein? Damit Kollision nicht mehr gezählt wird, wenn der Ball schon fast am Paddle vorbei ist.
    // Ball mit linkem Paddle
    if ((ball.y + ball.size > left_paddle.y && ball.y < left_paddle.y + left_paddle.height) && (ball.x <= left_paddle.x + left_paddle.width && ball.x >= left_paddle.x + left_paddle.width- tolerance))
    {
        ball.velocity_x *= -1;
        ball.x = left_paddle.x + left_paddle.width;
    }
    // Ball mit rechtem Paddle
    else if ((ball.y + ball.size > right_paddle.y && ball.y < right_paddle.y + right_paddle.height) && (ball.x + ball.size >= right_paddle.x && ball.x + ball.size <= right_paddle.x + tolerance))
    {
        ball.velocity_x *= -1;
        ball.x = right_paddle.x - ball.size;
    }




    // Ball mit linker "Wand"
    if (ball.x <= 0)
    {
        score_right_player++;
        game_is_ongoing = false;
    }
    // Ball mit rechter "Wand"
    else if (ball.x + ball.size >= window_width_units)
    {
        score_left_player++;
        game_is_ongoing = false;
    }


}

void draw_middle_lines(size_t num_segments)
{
    float segment_width = unit / 3.0;
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
    for (size_t i = 0; i < num_segments; i += 2)
    {
        SDL_Rect line = {.h = window_height_px / num_segments, .w = segment_width, .x = (window_width_px - segment_width) / 2.0, .y = i * window_height_px / num_segments};
        SDL_RenderFillRect(renderer, &line);
    }
}

void render()
{
    // Hintergrund
    SDL_SetRenderDrawColor(renderer, 0x00, 0x50, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    draw_middle_lines(15);

    // Game Objekte
    render_paddle(&left_paddle, renderer, unit);
    render_paddle(&right_paddle, renderer, unit);

    render_ball(&ball, renderer, unit);


    // Framebuffer aktualisieren
    SDL_RenderPresent(renderer);
}


void game_setup()
{
    left_paddle.y = (window_height_units - left_paddle.height) / 2;
    left_paddle.x = PADDLE_OFFSET;

    right_paddle.y = (window_height_units - right_paddle.height) / 2;
    right_paddle.x = (window_width_units - right_paddle.width - PADDLE_OFFSET);

    ball.x = (window_width_units - ball.size) / 2;
    ball.y = (window_height_units - ball.size) / 2;

    // Startrichtung des Balls setzen
    set_ball_direction(3);

    score_left_player = 0;
    score_right_player = 0;

    game_is_ongoing = true;
    render();
}

int main()
{
    // Initialisierung
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong Klon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    printf("unit: %f\n", unit);


    // Äussere Game Loop
    while(match_is_ongoing)
    {
        game_setup();
        SDL_WaitEvent(NULL);
        // Innere Game Loop
        while (game_is_ongoing)
        {
            render();
            handle_events();
            update();
        }

    }






    // Ressourcen freigeben
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

