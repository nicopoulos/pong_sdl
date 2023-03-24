#ifndef CONSTANTS_H
#define CONSTANTS_H


// Umgebung
#define INITIAL_WINDOW_WIDTH 1200
#define INITIAL_WINDOW_HEIGHT 800 // war 800

#define MAX_FPS 120
#define MIN_FRAME_DURATION (1000.0f / MAX_FPS)
#define MAX_FRAME_DURATION 100


// Grössen in Längeneinheiten (units)
#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 1
#define PADDLE_OFFSET 2
#define BALL_SIZE 1
#define BALL_VELOCITY_FACTOR 40 // Einheiten pro Sekunde
#define PADDLE_VELOCITY 40

// Mathe
#define PI 3.14159f
#define MAX_BOUNCE_OFF_ANGLE (PI / 4.0f) // 45°


// Schrift

#define SCORE_HEIGHT 5
#define SCORE_WIDTH 5
#define SCORE_HORIZONTAL_OFFSET 2
#define SCORE_VERTICAL_OFFSET 3



#endif