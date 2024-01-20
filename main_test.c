#include "board.h"
#include "player.h"
#include "control.h"
#include "display.h"
#include "score.h"
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

	initSettings();
#ifdef RASPI
    joyinfo_t joystick = {0, 0, J_NOPRESS};
#endif
	player_t player;
	initGame(&player);
	char key;

	do
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
		puts("I guess 'tis our goodbye then. Godspeed to you, traveler...");
	else
		puts("This is your end, cowboy...");

#ifndef RASPI
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
#endif
	return 0;
}
