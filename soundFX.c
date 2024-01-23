#include "soundFX.h"
#include <stdlib.h>
#include <stdio.h>

#define TRACK1 "music/AnalogueBubblebath.wav"
#define TRACK2 "music/Fingerbib.wav"
#define TRACK3 "music/Mookid.wav"
#define TRACK4 "music/PolynomialC.wav"
#define CLICK_FX "sounds/click.wav"
#define	LEVEL_UP_FX "sounds/levelUp.wav"

#ifdef RASPI
static Audio* music1; 
static Audio* music2;
static Audio* music3;
static Audio* music4;

static Audio* lock_sound;
static Audio* level_up_sound;

static Audio* aMusic[4];

// random index for the first song to be always random
static int indx;

void initSoundFX()
{
	music1 = createAudio(TRACK1, 0, SDL_MIX_MAXVOLUME);
	music2 = createAudio(TRACK2, 0, SDL_MIX_MAXVOLUME);
	music3 = createAudio(TRACK3, 0, SDL_MIX_MAXVOLUME);
	music4 = createAudio(TRACK4, 0, SDL_MIX_MAXVOLUME);

	lock_sound = createAudio(CLICK_FX, 0, SDL_MIX_MAXVOLUME);
	level_up_sound = createAudio(LEVEL_UP_FX, 0, SDL_MIX_MAXVOLUME);

	aMusic[0] = music1;
    aMusic[1] = music2;
    aMusic[2] = music3;
    aMusic[3] = music4;
	
	indx = rand() % 4; // given that srad is already initialized
	
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }
	
	playMusicFromMemory(aMusic[indx], SDL_MIX_MAXVOLUME);
}

void refreshMusic()
{
	if (playerStatus() == READY && musicStatus() == FINISHED) // (try only putting finished)
	{
		playMusicFromMemory(aMusic[(indx + 1) % 4], SDL_MIX_MAXVOLUME);
	}
}

void endSoundFX()
{
	endAudio();	
		
	freeAudio(music1);
	freeAudio(music2);
	freeAudio(music3);
	freeAudio(music4);
	freeAudio(lock_sound);
	freeAudio(level_up_sound);
}

void playLockSound()
{
	playSoundFromMemory(lock_sound, SDL_MIX_MAXVOLUME);
}

void playLevelUpSound()
{
	playSoundFromMemory(level_up_sound, SDL_MIX_MAXVOLUME);
}

#endif
