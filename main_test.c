#include "tetramino.h"
#include "board.h"
#include "game.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**************************************************************************/

int main(void)
{
	double fallInterval = 1; // en seg (rapidez inicial)
	double startTime, currentTime;
	startTime = getTime();

	srand((unsigned int)time(NULL)); // seed para rand()

	initSettings();

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
			performMove(&player, 's');
		}

		// rendering
		updateScene(&player);
#ifdef RASPI
		drawInDisplay();
#else
		drawScene();
		clearScreen();
#endif

		usleep(5000); // = 0.005 seg. para que renderize suavemente

		eraseLineIfFull();

	} while (!isGameOver() && key != 'x');

	if (key == 'x')
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
