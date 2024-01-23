#include "board.h"
#include "player.h"
#include "control.h"
#include "display.h"
#include "score.h"
#include "soundFX.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**************************************************************************/

int main(void)
{
	// timer	
	double fallInterval = getSpeed(0); // en seg (rapidez inicial nivel 0)
	double startTime, currentTime;
	startTime = getTime();

	// inicializaciones
    srand((unsigned int)time(NULL)); // seed para rand()
#ifdef RASPI
	joy_init();
	disp_init();
	disp_clear();
    joyinfo_t joystick = {0, 0, J_NOPRESS};
	initSoundFX();
#else
	enableNonBlockingInput(); // desactiva ICANON mode
#endif
	player_t player;
	initGame(&player); // espera input (nombre)
	char key;

	do // main loop
	{
		// control input
#ifdef RASPI
		joystick = joy_read();
		key = whichKeyWasPressed(&joystick);
		performMove(&player, key);
#else
		if (kbhit()) // se pregunta si se presiono una tecla
		{
			key = getchar();
			performMove(&player, key);
		}
#endif

		// caida libre
		currentTime = getTime();
		double elapsedTime = currentTime - startTime;

		if (elapsedTime >= fallInterval)
		{
			startTime = currentTime;
			performMove(&player, DOWN);
		}

		// score/points & level
		int linesCombo = eraseLineIfFull();
		player.lines += linesCombo;
		player.level = player.lines / 10;
		player.score += howMuchScore(player.level, linesCombo);

		// rendering
		updateScene(&player);
#ifdef RASPI
		drawInDisplay();
		refreshMusic(); // change music if finished
#else
		drawInTerminal(&player);
#endif

		// updates speed	
		if (player.level < MAX_LEVEL)
			fallInterval = getSpeed(player.level);
	
		// delay
		usleep(20000); // = 0.02 seg. para que renderize suavemente

	} while (!isGameOver() && key != EXIT);

	if (key == EXIT)
		printf("I guess 'tis our goodbye then. Godspeed to you, %s the quitter...\n", 
				player.name);
	else
		printf("This is thy end, %s the truthseeker...\n", player.name);

	// lista mejores puntajes
	updateTopScore("top10Score.dat", player.score, player.name);
	printTopScores();

	// free's y finalizacioens
#ifdef RASPI
	endSoundFX();
	disp_clear();
	disp_update();
#else
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
#endif
	return 0;
}
