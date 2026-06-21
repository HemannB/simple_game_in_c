#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

#define MUSIC_PATH "../assets/tetris.mp3"

typedef struct {
    Mix_Music *music;
} Audio;

int  audio_init(Audio *a);
void audio_destroy(Audio *a);
void audio_play_music(Audio *a);

#endif /* AUDIO_H */