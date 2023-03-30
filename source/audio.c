#include <SDL2/SDL.h>
#include <stdbool.h>

#include "audio.h"


int play_wav_procedure(void* ptr)
{
    char* file = (char*)ptr;
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8* wav_buffer;

    printf("before load wav\n");
    SDL_LoadWAV(file, &wav_spec, &wav_buffer, &wav_length);
    printf("after load wav\n");

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, false, &wav_spec, NULL, 0);
    printf("after open audio device\n");
    if (device_id == 0)
    {
        SDL_FreeWAV(wav_buffer);
        return 1;
    }
    printf("after if statement\n");

    SDL_QueueAudio(device_id, wav_buffer, wav_length);
    printf("after if statement\n");

    SDL_PauseAudioDevice(device_id, 0);
    printf("after pause audio device (%dms)\n", wav_length);
    SDL_Delay(5000);
    
    printf("before close audio device\n");
    SDL_CloseAudioDevice(device_id);
    printf("after close audio device\n");
    SDL_FreeWAV(wav_buffer);
    printf("after free wav\n");
    

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
