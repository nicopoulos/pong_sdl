#include "controller_handler.h"
#include "elements.h"

extern player_t left_player;
extern player_t right_player;


SDL_GameController* open_available_gamecontroller()
{
    printf("open_available_gamecontroller\n");
    SDL_GameController* controller = NULL;
    printf("Num Joysticks: %d\n", SDL_NumJoysticks());
    for (int index = 0; index < SDL_NumJoysticks(); index++)
    {
        if (SDL_GameControllerGetAttached(SDL_GameControllerFromPlayerIndex(index)) == SDL_FALSE)
        {
            controller = SDL_GameControllerOpen(index);
            printf("found unopened controller: %p\n", controller);
            break;
        }
    }
    

    return controller;
}



void on_controller_added(Sint32 joystick_index)
{
    printf("\ncontroller added. Index: %d\n", joystick_index);
    printf("left gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(left_player.gamepad), left_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(left_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(left_player.gamepad));
    
    printf("right gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(right_player.gamepad), right_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(right_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(right_player.gamepad));

    if (SDL_GameControllerGetAttached(left_player.gamepad) && SDL_GameControllerGetAttached(right_player.gamepad)){ /* do nothing */}
    else if (SDL_GameControllerGetAttached(left_player.gamepad))
    {
        right_player.gamepad = SDL_GameControllerOpen(1);
    }
    else if (SDL_GameControllerGetAttached(right_player.gamepad))
    {
        SDL_GameControllerClose(left_player.gamepad);
        left_player.gamepad = SDL_GameControllerOpen(1);
    }
    else
    {
        left_player.gamepad = SDL_GameControllerOpen(0);

    }


    printf("left gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(left_player.gamepad), left_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(left_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(left_player.gamepad));
    
    printf("right gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(right_player.gamepad), right_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(right_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(right_player.gamepad));

}

void on_controller_removed(SDL_JoystickID joystick_id)
{

    printf("\ncontroller removed. device ID: %d\n", joystick_id);

    printf("left gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(left_player.gamepad), left_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(left_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(left_player.gamepad));
    
    printf("right gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(right_player.gamepad), right_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(right_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(right_player.gamepad));




    if (SDL_GameControllerFromInstanceID(joystick_id) == left_player.gamepad)
    {
        SDL_GameControllerClose(left_player.gamepad);
        SDL_GameControllerClose(right_player.gamepad);
        right_player.gamepad = SDL_GameControllerOpen(0);

    }
    else if (SDL_GameControllerFromInstanceID(joystick_id) == right_player.gamepad)
    {
        SDL_GameControllerClose(left_player.gamepad);
        SDL_GameControllerClose(right_player.gamepad);
        left_player.gamepad = SDL_GameControllerOpen(0);
    }

    /*
    if (SDL_GameControllerFromInstanceID(joystick_id) == left_player.gamepad)
    {
        SDL_GameControllerClose(left_player.gamepad);
        SDL_GameControllerSetPlayerIndex(right_player.gamepad, 1);
    }
    else
    {
        SDL_GameControllerClose(right_player.gamepad);

    }

    */
    



    printf("left gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(left_player.gamepad), left_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(left_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(left_player.gamepad));
    
    printf("right gamepad: attached: %d, pointer: %p, Instance ID: %d, Player Index: %d\n", 
        SDL_GameControllerGetAttached(right_player.gamepad), right_player.gamepad, 
        SDL_JoystickGetDeviceInstanceID(SDL_GameControllerGetPlayerIndex(right_player.gamepad)), 
        SDL_GameControllerGetPlayerIndex(right_player.gamepad));

}

