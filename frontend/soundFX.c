#include "soundFX.h"
#include "control"
#include <stdlib.h>
#include <stdio.h>

#define TRACK5 "music/Mookid.wav"
#define TRACK0 "music/AnalogueBubblebath.wav"
#define TRACK1 "music/Fingerbib.wav"
#define TRACK2 "music/Flim.wav"
#define TRACK3 "music/PolynomialC.wav"
#define CLICK_FX "sounds/click1.wav"
#define	LEVEL_UP_FX "sounds/levelUp1.wav"
#define CLEAR_LINE_FX "sounds/lineClear1.wav"

#define T0_DURATION 286 // duracion del tema en segundos
#define T1_DURATION 229
#define T2_DURATION 117
#define T3_DURATION 287

#ifdef RASPI

static Audio *lock_sound;
static Audio *level_up_sound;
static Audio *line_cleared_sound;

static const char *aMusic[] = {TRACK0, TRACK1, TRACK2, TRACK3};
/*no guardo en memoria la musica pq es muy pesada, y si no se corre el codigo
hasta el final hay memory leaks grandes. en cambio, los sonidos si los guardo en 
memoria ya que se leen constantemente*/

// random index para que la primera cancion sea siempre random
static int indx;

void initSoundFX()
{
	// guardo en memoria los efectos de sonido	
	lock_sound = createAudio(CLICK_FX, 0, SDL_MIX_MAXVOLUME);
	if (lock_sound == NULL)
		fprintf(stderr, "Lock sound not initialized");
	level_up_sound = createAudio(LEVEL_UP_FX, 0, SDL_MIX_MAXVOLUME);
	if (level_up_sound == NULL)
		fprintf(stderr, "LvlUp sound not initialized");
	line_cleared_sound = createAudio(CLEAR_LINE_FX, 0, SDL_MIX_MAXVOLUME);	
	if (line_cleared_sound == NULL)
		fprintf(stderr, "Cleared line sound not initialized");

	indx = rand() % 4; // dado que srand ya se inicializo
	
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }
}

void startMusic()
{
	endAudio()
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }	
	playMusic(aMusic[indx], SDL_MIX_MAXVOLUME);
}

double refreshMusic(double startTime, int musicStatus)
{
	// tengo q hacer esta ranciedad pq SDL2 solo deja loopear infmente la musica 
	double currentTime = getTime();
	double elapsedTime = currentTime - startTime; // en segundos

	// duracion para cada tema
	switch (indx)
	{
	case 0: // track 0 = analogue bubblebath
		if (elapsedTime >= T0_DURATION)
		{
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			startTime = currentTime;
		}		
		break;
	case 1: // track 1 = fingerbib
		if (elapsedTime >= T1_DURATION)
		{
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			startTime = currentTime;
		}
		break;
	case 2: // track 2 = flim
		if (elapsedTime >= T2_DURATION)
		{
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			startTime = currentTime;
		}
		break;
	case 3: // track 3 = polynomial-c
		if (elapsedTime >= T3_DURATION)
		{
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			startTime = currentTime;
		}
		break;
	}
	return startTime;
}

void endSoundFX()
{
	endAudio();	

	freeAudio(lock_sound);
	freeAudio(level_up_sound);
	freeAudio(line_cleared_sound);
}

void playTitleScreenMusic()
{
	playMusic(TRACK5, SDL_MIX_MAXVOLUME);
}

void playLockSound()
{
	playSoundFromMemory(lock_sound, SDL_MIX_MAXVOLUME);
}

void playLevelUpSound()
{
	playSoundFromMemory(level_up_sound, SDL_MIX_MAXVOLUME);
}

void playLineClearSound()
{
	playSoundFromMemory(line_cleared_sound, SDL_MIX_MAXVOLUME);
}

#endif
