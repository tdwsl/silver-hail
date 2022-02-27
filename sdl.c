#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <assert.h>
#include "sdl.h"

SDL_Window *window;

void initSDL() {
  assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0);
  assert(window = SDL_CreateWindow("Silver Hail",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));

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
  SDL_DestroyWindow(window);
  SDL_Quit();
}
