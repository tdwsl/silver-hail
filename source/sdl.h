#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define sassert(C) assert(C)

extern SDL_Window *window;

extern Mix_Chunk *sfxShot1, *sfxShot2;
extern Mix_Chunk *sfxImpact1, *sfxImpact2, *sfxImpact3;

extern Mix_Music *tracks[];
extern int numTracks;

extern const Uint8 *keyboardState;

void initSDL();
void endSDL();
void toggleFullscreen();

#endif