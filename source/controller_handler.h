#ifndef CONTROLLER_HANDLER_H
#define CONTROLLER_HANDLER_H

#include <SDL2/SDL.h>


void on_controller_added(Sint32 joystick_index);

void on_controller_removed(SDL_JoystickID joystick_id);

#endif