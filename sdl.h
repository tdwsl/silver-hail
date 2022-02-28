#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern SDL_Window *window;

extern Mix_Chunk *sfxShot1, *sfxShot2;
extern Mix_Chunk *sfxImpact1, *sfxImpact2, *sfxImpact3;

void initSDL();
void endSDL();

#endif