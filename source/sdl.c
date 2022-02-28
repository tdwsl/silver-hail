#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <GL/gl.h>
#include <assert.h>
#include "sdl.h"

SDL_Window *window;

Mix_Chunk *sfxShot1, *sfxShot2, *sfxImpact1, *sfxImpact2, *sfxImpact3;

void initSDL() {
  assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0);
  assert(window = SDL_CreateWindow("Silver Hail",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));

  /* init mixer */
  assert(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) >= 0);

  /* load sound */
  assert(sfxShot1 = Mix_LoadWAV("sfx/shot1.wav"));
  assert(sfxShot2 = Mix_LoadWAV("sfx/shot2.wav"));
  assert(sfxImpact1 = Mix_LoadWAV("sfx/impact1.wav"));
  assert(sfxImpact2 = Mix_LoadWAV("sfx/impact2.wav"));
  assert(sfxImpact3 = Mix_LoadWAV("sfx/impact3.wav"));

  /* init opengl */
  SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glOrtho(0, 640, 480, 0, 0, 1);
  glClearColor(0, 0, 0, 1.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(3);
}

void endSDL() {
  Mix_FreeChunk(sfxShot2);
  Mix_FreeChunk(sfxShot1);

  Mix_CloseAudio();
  SDL_DestroyWindow(window);
  SDL_Quit();
}
