#include "soundFX.h"
#include <stdlib.h>
#include <stdio.h>

#define TRACK1 "music/AnalogueBubblebath.wav"
#define TRACK2 "music/Fingerbib.wav"
#define TRACK3 "music/Mookid.wav"
#define TRACK4 "music/PolynomialC.wav"
#define CLICK_FX "sounds/click1.wav"
#define	LEVEL_UP_FX "sounds/levelUp1.wav"
#define CLEAR_LINE_FX "sounds/lineClear1.wav"

#ifdef RASPI

static Audio *lock_sound;
static Audio *level_up_sound;
static Audio *line_cleared_sound;

static const char *aMusic[] = {TRACK1, TRACK2, TRACK3, TRACK4};
/*no guardo en memoria la musica pq es muy pesada, y si no se corre el codigo
hasta el final hay memory leaks grandes. en cambio, los sonidos si los guardo en 
memoria ya que se leen constantemente*/

// random index para que la primera cancion sea siempre random
static int indx;

void initSoundFX()
{
	// guardo en memoria los efectos de sonido	
	lock_sound = createAudio(CLICK_FX, 0, SDL_MIX_MAXVOLUME);
	level_up_sound = createAudio(LEVEL_UP_FX, 0, SDL_MIX_MAXVOLUME);
	line_cleared_sound = createAudio(CLEAR_LINE_FX, 0, SDL_MIX_MAXVOLUME);	

	indx = rand() % 4; // dado que srand ya se inicializo
	
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }
	
	playMusic(aMusic[indx], SDL_MIX_MAXVOLUME);
}

void refreshMusic()
{
	if (playerStatus() == READY && musicStatus() == FINISHED)
	{
		playMusic(aMusic[(indx + 1) % 4], SDL_MIX_MAXVOLUME);
	}
}

void endSoundFX()
{
	endAudio();	

	freeAudio(lock_sound);
	freeAudio(level_up_sound);
	freeAudio(line_cleared_sound);
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
