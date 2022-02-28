#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <assert.h>
#include "sdl.h"
#include "music.h"

extern bool paused, playerDead;

bool mute = false;

int currentTrack = 0;
bool newTrack = false;

void loadTrack(const char *filename) {
  //printf("loading %s\n", filename);
  assert(tracks[numTracks++] = Mix_LoadMUS(filename));
}

void nextTrack() {
#ifdef NOSOUND
  return;
#endif
  currentTrack++;
  currentTrack %= numTracks;

  newTrack = true;
}

void loadMusic() {
#ifdef NOSOUND
  return;
#endif
  loadTrack("music/track1.mp3");
  loadTrack("music/track2.mp3");
  loadTrack("music/track3.mp3");
  loadTrack("music/track4.mp3");
  loadTrack("music/track5.mp3");

  currentTrack = -1;
  nextTrack();
  Mix_PlayMusic(tracks[currentTrack], 0);
}

void updateMusic() {
#ifdef NOSOUND
  return;
#endif
  if(newTrack) {
    Mix_PlayMusic(tracks[currentTrack], 0);
    Mix_HookMusicFinished(nextTrack);
    newTrack = false;
  }

  if(paused || playerDead || mute)
    Mix_PauseMusic();
  else
    Mix_ResumeMusic();
}

void toggleMute() {
#ifdef NOSOUND
  return;
#endif
  mute = !mute;

  /*if(mute)
    Mix_VolumeMusic(0);
  else
    Mix_VolumeMusic(MIX_MAX_VOLUME);*/
}