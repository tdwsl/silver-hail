#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <GL/gl.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sdl.h"

SDL_Window *window;
SDL_GLContext context;

const Uint8 *keyboardState;

Mix_Chunk *sfxShot1, *sfxShot2, *sfxImpact1, *sfxImpact2, *sfxImpact3;

Mix_Music *tracks[20];
int numTracks = 0;

bool fullscreen = true;

/* called on init, as well as toggle fullscreen */
void initGL() {
  SDL_GL_DeleteContext(context);
  context = SDL_GL_CreateContext(window);
  glClearColor(0, 0, 0, 1.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(3);
  glOrtho(0, 640, 480, 0, 0, 1);
}

void initSDL() {
  const int init = SDL_INIT_VIDEO | SDL_INIT_AUDIO
      | SDL_INIT_TIMER | SDL_INIT_EVENTS;

  assert(SDL_Init(init) >= 0);
  assert(window = SDL_CreateWindow("Silver Hail",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));

  keyboardState = SDL_GetKeyboardState(NULL);

#ifndef NOSOUND
  /* init mixer */
  assert(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
      2, 4096) >= 0);

  /* load sfx */
  assert(sfxShot1 = Mix_LoadWAV("sfx/shot1.wav"));
  assert(sfxShot2 = Mix_LoadWAV("sfx/shot2.wav"));
  assert(sfxImpact1 = Mix_LoadWAV("sfx/impact1.wav"));
  assert(sfxImpact2 = Mix_LoadWAV("sfx/impact2.wav"));
  assert(sfxImpact3 = Mix_LoadWAV("sfx/impact3.wav"));
#endif

  /* init opengl */
  context = SDL_GL_CreateContext(window);
  initGL();

  fullscreen = true;
  toggleFullscreen();
}

void toggleFullscreen() {
  fullscreen = !fullscreen;

  if(!fullscreen) {
    SDL_SetWindowFullscreen(window, 0);
    initGL();

    glViewport(0, 0, 640, 480);
  }
  else {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    initGL();

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    float xs = (float)w/640.0, ys = (float)h/480.0;
    float s = (xs > ys) ? ys : xs;
    glViewport(w/2-s*640.0/2, h/2-s*480.0/2, s*640, s*480);
  }
}

void endSDL() {
#ifndef NOSOUND
  /* free music */
  for(int i = 0; i < numTracks; i++)
    Mix_FreeMusic(tracks[i]);
  numTracks = 0;

  /* free sfx */
  Mix_FreeChunk(sfxImpact1);
  Mix_FreeChunk(sfxImpact2);
  Mix_FreeChunk(sfxImpact3);
  Mix_FreeChunk(sfxShot2);
  Mix_FreeChunk(sfxShot1);

  Mix_CloseAudio();
#endif

  SDL_DestroyWindow(window);
  SDL_Quit();
}
