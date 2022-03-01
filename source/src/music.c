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
  tracks[numTracks++] = Mix_LoadMUS(filename);
  if(!tracks[numTracks-1]) {
    printf("%s%s\nloading %s failed\n", Mix_GetError(), SDL_GetError());
  }
}

void nextTrack() {
  currentTrack++;
  currentTrack %= numTracks;

  newTrack = true;
}

void loadMusic() {
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
  mute = !mute;

  /*if(mute)
    Mix_VolumeMusic(0);
  else
    Mix_VolumeMusic(MIX_MAX_VOLUME);*/
}