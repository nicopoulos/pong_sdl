#include <SDL2/SDL.h>
#include <stdbool.h>

#include "audio.h"


int play_wav_procedure(void* ptr)
{
    char* file = (char*)ptr;
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8* wav_buffer;

    SDL_LoadWAV(file, &wav_spec, &wav_buffer, &wav_length);

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, false, &wav_spec, NULL, 0);
    if (device_id == 0)
    {
        SDL_FreeWAV(wav_buffer);
        return 1;
    }

    SDL_QueueAudio(device_id, wav_buffer, wav_length);

    SDL_PauseAudioDevice(device_id, 0);
    SDL_Delay(5000);
    
    SDL_CloseAudioDevice(device_id);
    SDL_FreeWAV(wav_buffer);
    

    return 0;

}



int play_wav(const char* file)
{
    SDL_Thread* thread = SDL_CreateThread(play_wav_procedure, "sound_thread", (void*)file);
    if (thread == NULL)
        return 1;
    else
        return 0;
}
