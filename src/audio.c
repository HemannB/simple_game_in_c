#include "audio.h"
#include <stdio.h>

int audio_init(Audio *a)
{
    a->music = NULL;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
        return 0;
    }

    a->music = Mix_LoadMUS(MUSIC_PATH);
    if (!a->music) {
        fprintf(stderr, "Mix_LoadMUS error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        return 0;
    }

    return 1;
}

void audio_play_music(Audio *a)
{
    // -1 = loop infinito
    if (Mix_PlayMusic(a->music, -1) != 0)
        fprintf(stderr, "Mix_PlayMusic error: %s\n", Mix_GetError());
}

void audio_destroy(Audio *a)
{
    if (a->music)
        Mix_FreeMusic(a->music);
    Mix_CloseAudio();
}