#include "soundFX.h"
#include "control.h"
#include <stdlib.h>
#include <stdio.h>



#define TRACK5 "music/15AutumnTravels.wav"
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
#elif ALLEGRO
static ALLEGRO_SAMPLE *lock_sound;
static ALLEGRO_SAMPLE *level_up_sound;
static ALLEGRO_SAMPLE *line_cleared_sound;
static ALLEGRO_VOICE *voice;
#endif
static const char *aMusic[] = {TRACK0, TRACK1, TRACK2, TRACK3};
/*no guardo en memoria la musica pq es muy pesada, y si no se corre el codigo
hasta el final hay memory leaks grandes. en cambio, los sonidos si los guardo en 
memoria ya que se leen constantemente*/

// random index para que la primera cancion sea siempre random
static int indx;

void initSoundFX()
{
	// guardo en memoria los efectos de sonido	
	#ifdef RASPI
	lock_sound = createAudio(CLICK_FX, 0, SDL_MIX_MAXVOLUME);
	if (lock_sound == NULL)
		fprintf(stderr, "Lock sound not initialized");
	level_up_sound = createAudio(LEVEL_UP_FX, 0, SDL_MIX_MAXVOLUME);
	if (level_up_sound == NULL)
		fprintf(stderr, "LvlUp sound not initialized");
	line_cleared_sound = createAudio(CLEAR_LINE_FX, 0, SDL_MIX_MAXVOLUME);	
	if (line_cleared_sound == NULL)
		fprintf(stderr, "Cleared line sound not initialized");
	#elif ALLEGRO
	lock_sound = al_load_sample(CLICK_FX);
	if (lock_sound == NULL)
		fprintf(stderr, "Lock sound not initialized");
	level_up_sound = al_load_sample(LEVEL_UP_FX);
	if (level_up_sound == NULL)
		fprintf(stderr, "LvlUp sound not initialized");
	line_cleared_sound = al_load_sample(CLEAR_LINE_FX);	
	if (line_cleared_sound == NULL)
		fprintf(stderr, "Cleared line sound not initialized");
		voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (!voice) {
        fprintf(stderr, "Failed to create voice\n");
        return;
    }
	#endif
	
	indx = rand() % 4; // dado que srand ya se inicializo
	#ifdef RASPI
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }
	#endif
}

void startMusic()
{
	#ifdef RASPI
	endAudio();
	if (initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
		endAudio();
    }	
	playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
	#elif ALLEGRO
	al_stop_samples();
	allePlaySong(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
	#endif
}

double refreshMusic(double startTime)
{
	double currentTime = getTime();
	double elapsedTime = currentTime - startTime; // en segundos

	// duracion para cada tema
	switch (indx)
	{
	case 0: // track 0 = analogue bubblebath
		if (elapsedTime >= T0_DURATION)
		{
			#ifdef RASPI
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#elif ALLEGRO
			allePlaySong(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#endif
			startTime = currentTime;
		}		
		break;
	case 1: // track 1 = fingerbib
		if (elapsedTime >= T1_DURATION)
		{
			#ifdef RASPI
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#elif ALLEGRO
			allePlaySong(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#endif
			startTime = currentTime;
		}
		break;
	case 2: // track 2 = flim
		if (elapsedTime >= T2_DURATION)
		{
			#ifdef RASPI
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#elif ALLEGRO
			allePlaySong(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#endif
			startTime = currentTime;
		}
		break;
	case 3: // track 3 = polynomial-c
		if (elapsedTime >= T3_DURATION)
		{
			#ifdef RASPI
			playMusic(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#elif ALLEGRO
			allePlaySong(aMusic[++indx % 4], SDL_MIX_MAXVOLUME);
			#endif
			startTime = currentTime;
		}
		break;
	}
	return startTime;
}


void endSoundFX()
{
	#ifdef RASPI
	endAudio();	

	freeAudio(lock_sound);
	freeAudio(level_up_sound);
	freeAudio(line_cleared_sound);
	#elif ALLEGRO
	al_destroy_voice(voice);
	#endif
}

void playTitleScreenMusic()
{
	#ifdef RASPI
	playMusic(TRACK5, SDL_MIX_MAXVOLUME);
	#elif ALLEGRO
	allePlaySong(TRACK5, SDL_MIX_MAXVOLUME);
	#endif
}

void playLockSound()
{
	#ifdef RASPI
	playSoundFromMemory(lock_sound, SDL_MIX_MAXVOLUME);
	#elif ALLEGRO
	allePlaySound(lock_sound, SDL_MIX_MAXVOLUME);
	#endif
}

void playLevelUpSound()
{
	#ifdef RASPI
	playSoundFromMemory(level_up_sound, SDL_MIX_MAXVOLUME);
	#elif ALLEGRO
	allePlaySound(level_up_sound, SDL_MIX_MAXVOLUME);
	#endif
	
}

void playLineClearSound()
{
	#ifdef RASPI
	playSoundFromMemory(line_cleared_sound, SDL_MIX_MAXVOLUME);
	#elif ALLEGRO
	allePlaySound(line_cleared_sound, SDL_MIX_MAXVOLUME);
	#endif
	
}

#ifdef ALLEGRO
void allePlaySong(const char *filename, int volume) {
    ALLEGRO_MIXER *mixer = al_get_default_mixer();
    

    al_attach_mixer_to_voice(mixer, voice);
    al_set_mixer_playing(mixer, true);

    ALLEGRO_SAMPLE *song = al_load_sample(filename);

    if (song) {
        // Map the volume from the range [0, 128] to [0.0, 1.0]
        float mappedVolume = (float)volume / 128.0;

        al_play_sample(song, mappedVolume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else {
        fprintf(stderr, "Failed to load song: %s\n", filename);
    }
}

void allePlaySound(ALLEGRO_SAMPLE *sample, int volume) {
    ALLEGRO_MIXER *mixer = al_get_default_mixer();
    

    al_attach_mixer_to_voice(mixer, voice);
    al_set_mixer_playing(mixer, true);

    if (sample) {
        // Mapeo el volumen de los int [0, 128] a float [0.0, 1.0], asi se integra mejor con las funciones de la raspi
        float mappedVolume = (float)volume / 128.0;

        al_play_sample(sample, mappedVolume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else {
        fprintf(stderr, "Error al cargar sonido\n");
    }
    
}

#endif

