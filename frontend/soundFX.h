#ifndef SOUNDFX_H
#define SOUNDFX_H

#include "audio.h"

// call after srand is initiliazed
void initSoundFX();

void startMusic();

double refreshMusic(double);

void endSoundFX();

void playTitleScreenMusic();

void playLockSound();

void playLevelUpSound();

void playLineClearSound();

void playGameOverSound();

void stopMusic();

#endif // SOUNDFX_H

