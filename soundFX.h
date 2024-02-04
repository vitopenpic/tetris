#ifndef SOUNDFX_H
#define SOUNDFX_H
#include "audio.h"
#ifdef ALLEGRO
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#endif
// call after srand is initiliazed
void initSoundFX();

void startMusic();

double refreshMusic(double);


void endSoundFX();

void playTitleScreenMusic();

void playLockSound();

void playLevelUpSound();

void playLineClearSound();

#ifdef ALLEGRO
/* play sounds in allegro
@param allegro sample variable with the sound and the volume ranging from 0 to 128
*/
void allePlaySound(ALLEGRO_SAMPLE *sample, int volume);
/* play music in allegro
@param path to the song that it has to play and the volume ranging from 0 to 128
*/
void allePlaySong(const char *filename, int volume);

#endif
#endif // SOUNDFX_H
