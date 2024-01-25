#ifndef SOUNDFX_H
#define SOUNDFX_H

#include "audio.h"

// call after srand is initiliazed
void initSoundFX();

void refreshMusic();

void endSoundFX();

void playTitleScreenMusic();

void playLockSound();

void playLevelUpSound();

void playLineClearSound();

#endif // SOUNDFX_H
