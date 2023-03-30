#include "game.h"
#include "elements.h"
#include "constants.h"

#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_image.h>

// constants
#define SCORE_TO_WIN 11

#define PADDLE_OFFSET 2
#define MAX_FPS 120
#define MIN_FRAME_DURATION 1000.0 / MAX_FPS
#define MAX_FRAME_DURATION 100

#define SCORE_HEIGHT 5
#define SCORE_WIDTH 5
#define SCORE_HORIZONTAL_OFFSET 2
#define SCORE_VERTICAL_OFFSET 3

#define NUM_MIDDLE_LINES 5

#define BALL_VELOCITY_FACTOR 40
#define PADDLE_VELOCITY 40
#define MAX_BOUNCE_OFF_ANGLE (PI / 4.0) // 45°


// extern variables
extern SDL_Window* screen;
extern SDL_Renderer* renderer;

extern int window_width;
extern int window_height;

extern double unit;
extern TTF_Font* main_font;

// local variables
double window_width_units;
double window_height_units;
bool game_running;
bool ball_served;
bool match_ongoing;
bool start_new_game_after_exit;
SDL_Surface* temp;



// objects
paddle_t left_paddle;
paddle_t right_paddle;
const paddle_t * serving_paddle;
ball_t ball;


enum player {LEFT_PLAYER, RIGHT_PLAYER};


int setup_game();
int render_game();
int update_game();
int game_input();
int quit_game();
int check_win_condition();


int on_ball_serve();
int win_overlay(int winner);


int draw_middle_lines(unsigned int num_lines);
double get_rand_double(double min, double max);



int pause_overlay();

// only public function
int game()
{
    start_new_game_after_exit = false;
    srand((unsigned int)time(NULL));
    window_height_units = window_height / unit;
    window_width_units = window_width / unit;

    left_paddle.height = window_height_units / 6.0;
    left_paddle.width = 1;
    left_paddle.score.count = 0;
    left_paddle.score.length = 1;
    snprintf(left_paddle.score.string, 10, "%lu", left_paddle.score.count);

    right_paddle = left_paddle;
    left_paddle.pos_x = PADDLE_OFFSET;
    right_paddle.pos_x = window_width_units - PADDLE_OFFSET - right_paddle.width;

    ball.size = 1;


    game_running = true;
    // game loop
    while(game_running)
    {
        setup_game();
            // implement serve
        while (match_ongoing)
        {
            render_game();
            game_input();
            check_win_condition();
            SDL_RenderPresent(renderer);
            update_game();
        }
    }


    // clean up
    SDL_FreeSurface(temp);

    return start_new_game_after_exit;
}


int setup_game()
{
    left_paddle.pos_y = (window_height_units - left_paddle.height) / 2.0;
    right_paddle.pos_y = left_paddle.pos_y;

    ball.pos_x = (window_width_units - ball.size) / 2.0;
    ball.pos_y = (window_height_units - ball.size) / 2.0;

    if ((left_paddle.score.count + right_paddle.score.count) % 2 == 0)
    {
        serving_paddle = &left_paddle;
        ball.pos_x = left_paddle.pos_x + left_paddle.width;
    }
    else
    {
        serving_paddle = &right_paddle;
        ball.pos_x = right_paddle.pos_x - ball.size;
    }

    // set input booleans to false
    left_paddle.upwards = false;
    left_paddle.downwards = false;
    right_paddle.upwards = false;
    right_paddle.downwards = false;

    match_ongoing = true;
    ball_served = false;

    render_game();

    return 0;
}

int game_input()
{
 SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE:
                        pause_overlay();
                        break;
                    case SDL_SCANCODE_W:
                    {
                        left_paddle.upwards = true;
                        break;
                    }
                    case SDL_SCANCODE_S:
                    {
                        left_paddle.downwards = true;
                        break;
                    }
                    case SDL_SCANCODE_UP:
                        right_paddle.upwards = true;
                        break;
                    case SDL_SCANCODE_DOWN:
                        right_paddle.downwards = true;
                        break;
                    case SDL_SCANCODE_SPACE:
                    {
                        if (ball_served == false)
                            on_ball_serve();
                        break;
                    }

                    default:
                        break;

                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    {
                        left_paddle.upwards = false;
                        break;
                    }
                    case SDL_SCANCODE_S:
                    {
                        left_paddle.downwards = false;
                        break;
                    }
                    case SDL_SCANCODE_UP:
                        right_paddle.upwards = false;
                        break;
                    case SDL_SCANCODE_DOWN:
                        right_paddle.downwards = false;
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }

    }



    return 0;
}

int update_game()
{

    // handle frame duration
    static Uint32 last_frame = 0;
    double delta_time = (double)(SDL_GetTicks64() - last_frame);

    if (delta_time < MIN_FRAME_DURATION)
        SDL_Delay((last_frame + MIN_FRAME_DURATION) - SDL_GetTicks64());
    else if (delta_time > MAX_FRAME_DURATION)
        delta_time = MAX_FRAME_DURATION;
    
    double frame_duration_s = delta_time / 1000.0;
    last_frame = SDL_GetTicks64();

    // new ball position

    if (ball_served == false)
    {
        ball.pos_y = serving_paddle->pos_y + ((serving_paddle->height - ball.size) / 2.0);
    }
    else
    {
        ball.pos_x += ball.velocity_x * BALL_VELOCITY_FACTOR * frame_duration_s;
        ball.pos_y += ball.velocity_y * BALL_VELOCITY_FACTOR * frame_duration_s;
    }


    // left paddle up
    if (left_paddle.upwards && !left_paddle.downwards)
        left_paddle.pos_y -= PADDLE_VELOCITY * frame_duration_s;
    // left paddle down
    if (left_paddle.downwards && !left_paddle.upwards)
        left_paddle.pos_y += PADDLE_VELOCITY * frame_duration_s;

    // right paddle up
    if (right_paddle.upwards && !right_paddle.downwards)
        right_paddle.pos_y -= PADDLE_VELOCITY * frame_duration_s;
    
    // right paddle down
    if (right_paddle.downwards && !right_paddle.upwards)
        right_paddle.pos_y += PADDLE_VELOCITY * frame_duration_s;
    



    // collisiondetection

    // left paddle with walls
    if (left_paddle.pos_y <= 0)
        left_paddle.pos_y = 0;
    else if (left_paddle.pos_y >= window_height_units - right_paddle.height)
        left_paddle.pos_y = window_height_units - right_paddle.height;
    
    // right paddle with walls
    if (right_paddle.pos_y <= 0)
        right_paddle.pos_y = 0;
    else if (right_paddle.pos_y >= window_height_units - right_paddle.height)
        right_paddle.pos_y = window_height_units - right_paddle.height;

    // ball top wall
    if (ball.pos_y <= 0)
    {
        ball.velocity_y *= -1;
        ball.pos_y = 0;
    }
    // ball with bottom wall
    else if (ball.pos_y + ball.size >= window_height_units)
    {
        ball.velocity_y *= -1;
        ball.pos_y = window_height_units - ball.size;
    }

    double tolerance = (double)ball.size; // should be higher, the faster the ball
    // ball with left paddle
    if ((ball.pos_y + ball.size > left_paddle.pos_y && ball.pos_y < left_paddle.pos_y + left_paddle.height) && ball.pos_x <= left_paddle.pos_x + left_paddle.width && ball.pos_x >= left_paddle.pos_x + left_paddle.width - tolerance)
    {
        // depending on position on the paddle, the ball should move with a different angle
        double paddle_center_y = left_paddle.pos_y + (left_paddle.height / 2.0);
        double ball_center_y = ball.pos_y + (ball.size / 2.0);
        double offset = ball_center_y - paddle_center_y;
        double bounce_off_factor = offset / (left_paddle.height / 2.0);
        if (bounce_off_factor > 1)
            bounce_off_factor = 1;
        double bounce_off_angle = bounce_off_factor * MAX_BOUNCE_OFF_ANGLE;
        set_ball_direction(&ball, bounce_off_angle);
    }
    // ball with right paddle
    else if ((ball.pos_y + ball.size > right_paddle.pos_y && ball.pos_y < right_paddle.pos_y + right_paddle.height) && (ball.pos_x + ball.size >= right_paddle.pos_x && ball.pos_x + ball.size <= right_paddle.pos_x + tolerance))
    {
        double paddle_center_y = right_paddle.pos_y + (right_paddle.height / 2.0);
        double ball_center_y = ball.pos_y + (ball.size / 2.0);
        double offset = ball_center_y - paddle_center_y;
        double bounce_off_factor = offset / (right_paddle.height / 2.0);
        if (bounce_off_factor > 1)
            bounce_off_factor = 1;
        double bounce_off_angle = PI - (bounce_off_factor * MAX_BOUNCE_OFF_ANGLE);
        set_ball_direction(&ball, bounce_off_angle);
    }

    // ball with left "wall"
    if (ball.pos_x <= 0)
    {
        right_paddle.score.count++;
        snprintf(right_paddle.score.string, 10, "%lu", right_paddle.score.count);
        right_paddle.score.length = strlen(right_paddle.score.string);
        match_ongoing = false;
    }
    // ball with right "wall"
    else if (ball.pos_x + ball.size >= window_width_units)
    {
        left_paddle.score.count++;
        snprintf(left_paddle.score.string, 10, "%lu", left_paddle.score.count);
        left_paddle.score.length = strlen(left_paddle.score.string);
        match_ongoing = false;
    }



    return 0;
}

int render_game()
{
    // background
    SDL_SetRenderDrawColor(renderer, 0x00, 0x50, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // draw_middle_lines(15);
    draw_middle_lines(NUM_MIDDLE_LINES);

    // score
    temp = TTF_RenderText_Solid(main_font, left_paddle.score.string, (SDL_Color){0xFF, 0xFF, 0xFF});
    SDL_Texture* left_player_score_texture = SDL_CreateTextureFromSurface(renderer, temp);
    temp = TTF_RenderText_Solid(main_font, right_paddle.score.string, (SDL_Color){0xFF, 0xFF, 0xFF});
    SDL_Texture* right_player_score_texture = SDL_CreateTextureFromSurface(renderer, temp);

    SDL_Rect left_score_rect = {.h = SCORE_HEIGHT * unit, .w = left_paddle.score.length * SCORE_WIDTH * unit, .x = unit * (window_width_units / 2.0 - SCORE_HORIZONTAL_OFFSET - SCORE_WIDTH * left_paddle.score.length), .y = SCORE_VERTICAL_OFFSET * unit};

    SDL_Rect right_score_rect = {.h = SCORE_HEIGHT * unit, .w = right_paddle.score.length * SCORE_WIDTH * unit, .x = unit * (window_width_units / 2.0 + SCORE_HORIZONTAL_OFFSET), .y = SCORE_VERTICAL_OFFSET * unit};

    SDL_RenderCopy(renderer, left_player_score_texture, NULL, &left_score_rect);
    SDL_RenderCopy(renderer, right_player_score_texture, NULL, &right_score_rect);

    SDL_DestroyTexture(left_player_score_texture);
    SDL_DestroyTexture(right_player_score_texture);

    // game objects
    render_paddle(&left_paddle, renderer, unit);
    render_paddle(&right_paddle, renderer, unit);

    render_ball(&ball, renderer, unit);


    // show new frame


    return 0;
}


int quit_game()
{
    match_ongoing = false;
    game_running = false;

    return 0;
}

int on_ball_serve()
{
    double angle = get_rand_double(-MAX_BOUNCE_OFF_ANGLE, MAX_BOUNCE_OFF_ANGLE);
    if (serving_paddle == &left_paddle)
    {
        set_ball_direction(&ball, angle);
    }
    else
    {
        set_ball_direction(&ball, PI + angle);
    }

    ball_served = true;


    return 0;

}

int draw_middle_lines(unsigned int num_lines)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int num_segments = num_lines * 2 - 1;
    double segment_length = window_height / num_segments;
    double segment_width = unit / 3.0;
    for (int i = 0; i < num_segments; i+= 2)
    {
        SDL_Rect rect = {.h = segment_length, .w = segment_width, .x = (window_width - segment_width) / 2.0, .y = segment_length * i};
        SDL_RenderFillRect(renderer, &rect);

    }

    return 0;
}


double get_rand_double(double min, double max)
{
    double fractional = (double)rand() / ((double)RAND_MAX);
    double diff = max - min;
    double angle = min + (diff * fractional);

    return angle;
}


int check_win_condition()
{
    if (left_paddle.score.count == SCORE_TO_WIN)
        win_overlay(LEFT_PLAYER);
    else if (right_paddle.score.count == SCORE_TO_WIN)
        win_overlay(RIGHT_PLAYER);
    
    return 0;
}





// GUI



// pause menu
#define NUM_PAUSE_BUTTONS 3
button_t pause_buttons[NUM_PAUSE_BUTTONS];
int selected_button_idx;

SDL_Texture* court_screenshot;
bool close_overlay;

int render_overlay()
{
    // overlay window
    // background (court)

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, court_screenshot, NULL, NULL);

    // buttons
    for (int i = 0; i < NUM_PAUSE_BUTTONS; i++)
    {
        render_button(renderer, &(pause_buttons[i]));
    }

    SDL_RenderPresent(renderer);

    return 0;
}

int overlay_input()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    switch(event.type)
    {
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                {
                    if (selected_button_idx != 0)
                    {
                        pause_buttons[selected_button_idx].selected = false;
                        selected_button_idx--;
                        pause_buttons[selected_button_idx].selected = true;
                    }

                    break;
                }

                case SDL_SCANCODE_DOWN:
                {
                    if (selected_button_idx != NUM_PAUSE_BUTTONS - 1)
                    {
                        pause_buttons[selected_button_idx].selected= false;
                        selected_button_idx++;
                        pause_buttons[selected_button_idx].selected = true;
                    }

                    break;
                }

                case SDL_SCANCODE_RETURN:
                {
                    switch(selected_button_idx)
                    {
                        case 0: // resume
                        {
                            close_overlay = true;
                            break;
                        }
                        case 1: // restart
                        {
                            game_running = false;
                            match_ongoing = false;
                            close_overlay = true;
                            start_new_game_after_exit = true;
                            break;
                        }
                        case 2: // return to menu
                        { 
                            game_running = false;
                            match_ongoing = false;
                            close_overlay = true;

                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                default:
                {
                    break;
                }
            }
            break;
        }

    }

    return 0;

}

int pause_overlay()
{
    double padding = window_height / 15.0;

    pause_buttons[1].rect.h = window_height / 6.0;
    pause_buttons[1].rect.w = window_width / 2.5;
    pause_buttons[1].rect.x = (window_width - pause_buttons[1].rect.w) / 2.0;
    pause_buttons[1].rect.y = (window_height - pause_buttons[1].rect.h) / 2.0;
    pause_buttons[1].font = main_font;
    pause_buttons[1].margin = pause_buttons[1].rect.h / 8.0;
    pause_buttons[1].selected = false;

    snprintf(pause_buttons[1].text, 20, "Neustarten");

    pause_buttons[0] = pause_buttons[1];
    pause_buttons[0].rect.y = pause_buttons[1].rect.y - pause_buttons[1].rect.h - padding;
    pause_buttons[0].selected = true;
    snprintf(pause_buttons[0].text, 20, "Fortsetzen");


    pause_buttons[2] = pause_buttons[1];
    pause_buttons[2].rect.y = pause_buttons[1].rect.y + pause_buttons[1].rect.h + padding;
    snprintf(pause_buttons[2].text, 20, "Zum Homescreen");

    selected_button_idx = 0;

    const Uint32 format = SDL_PIXELFORMAT_ABGR8888;

    SDL_Surface* scrsht = SDL_CreateRGBSurfaceWithFormat(0, window_width, window_height, 32, format);
    if (scrsht == NULL)
    {
        fprintf(stderr, "error creating surface\n");
        return 1;
    }
    SDL_RenderReadPixels(renderer, NULL, format, scrsht->pixels, scrsht->pitch);
    court_screenshot = SDL_CreateTextureFromSurface(renderer, scrsht);
    if (court_screenshot == NULL)
    {

        fprintf(stderr, "error creating texture\n");
        return 1;
    }
    

    close_overlay = false;
    // Rendering
    while(close_overlay == false)
    {
        render_overlay();
        overlay_input();
    }
    SDL_FreeSurface(scrsht);
    SDL_DestroyTexture(court_screenshot);


    
    return 0;
}




// win overlay
#define NUM_WIN_BUTTONS 2
button_t win_buttons[NUM_WIN_BUTTONS];

SDL_Texture* win_title_texture;
SDL_Rect win_title_rect;

SDL_Texture* trophy_texture;
SDL_Rect trophy_rect;


int win_overlay_input()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    switch(event.type)
    {
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                {
                    if (selected_button_idx != 0)
                    {
                        win_buttons[selected_button_idx].selected = false;
                        selected_button_idx--;
                        win_buttons[selected_button_idx].selected = true;
                    }

                    break;
                }

                case SDL_SCANCODE_DOWN:
                {
                    if (selected_button_idx != NUM_WIN_BUTTONS - 1)
                    {
                        win_buttons[selected_button_idx].selected= false;
                        selected_button_idx++;
                        win_buttons[selected_button_idx].selected = true;
                    }

                    break;
                }

                case SDL_SCANCODE_RETURN:
                {
                    switch(selected_button_idx)
                    {
                        case 0: // restart
                        {
                            game_running = false;
                            match_ongoing = false;
                            close_overlay = true;
                            start_new_game_after_exit = true;
                            break;
                        }
                        case 1: // return to menu
                        { 
                            game_running = false;
                            match_ongoing = false;
                            close_overlay = true;

                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                default:
                {
                    break;
                }
            }
            break;
        }

    }

    return 0;
}

int render_win_overlay()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, court_screenshot, NULL, NULL);

    for(int i = 0; i < NUM_WIN_BUTTONS; i++)
    {
        render_button(renderer, &(win_buttons[i]));
    }

    SDL_RenderCopy(renderer, trophy_texture, NULL, &trophy_rect);

    SDL_RenderPresent(renderer);

    return 0;
}

int win_overlay(int winner)
{
    double padding = window_height / 20.0;


    // buttons
    win_buttons[0].rect.h = window_height / 6.0;
    win_buttons[0].rect.w = window_width / 3.0;
    if (winner == LEFT_PLAYER)
        win_buttons[0].rect.x = (window_width / 2.0 - win_buttons[0].rect.w) / 2.0;
    else
        win_buttons[0].rect.x = (window_width / 2.0) + ((window_width / 2.0 - win_buttons[0].rect.w) / 2.0);

    win_buttons[0].rect.y = (window_height - win_buttons[0].rect.h ) / 2.0;
    win_buttons[0].font = main_font;
    win_buttons[0].margin = win_buttons[0].rect.h / 8.0;
    win_buttons[0].selected = true;

    snprintf(win_buttons[0].text, 20, "Neues Spiel");

    win_buttons[1] = win_buttons[0];
    win_buttons[1].rect.y = win_buttons[0].rect.y + win_buttons[0].rect.h + padding;
    win_buttons[1].selected = false;
    snprintf(win_buttons[1].text, 20, "Zum Homescreen");

    selected_button_idx = 0;

    // trophy
    trophy_texture = IMG_LoadTexture(renderer, "assets/trophy.png");
    trophy_rect.h = window_height / 5.0;
    trophy_rect.w = window_width / 5.0;
    trophy_rect.x = win_buttons[0].rect.x + ((win_buttons[0].rect.w - trophy_rect.w) / 2.0);
    trophy_rect.y = win_buttons[0].rect.y - trophy_rect.h;


    const Uint32 format = SDL_PIXELFORMAT_ABGR8888;

    SDL_Surface* scrsht = SDL_CreateRGBSurfaceWithFormat(0, window_width, window_height, 32, format);
    if (scrsht == NULL)
    {
        fprintf(stderr, "error creating surface\n");
        return 1;
    }
    SDL_RenderReadPixels(renderer, NULL, format, scrsht->pixels, scrsht->pitch);
    court_screenshot = SDL_CreateTextureFromSurface(renderer, scrsht);
    if (court_screenshot == NULL)
    {

        fprintf(stderr, "error creating texture\n");
        return 1;
    }
    

    close_overlay = false;
    // Rendering
    while(close_overlay == false)
    {
        render_win_overlay();
        win_overlay_input();
    }

    SDL_FreeSurface(scrsht);
    SDL_DestroyTexture(court_screenshot);
    SDL_DestroyTexture(win_title_texture);
    SDL_DestroyTexture(trophy_texture);


    
    return 0;
    // new game
    // menu

}