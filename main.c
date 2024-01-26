#include "backend/board.h"
#include "backend/player.h"
#include "backend/score.h"
#include "backend/menu.h"
#include "frontend/control.h"
#include "frontend/display.h"
#include "frontend/soundFX.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**************************************************************************/

int main(void)
{
	// inicializaciones
    srand((unsigned int)time(NULL)); // seed para rand()
	player_t player;
	char key;
#ifdef RASPI
	joy_init();
	disp_init();
    joyinfo_t joystick = {0, 0, J_NOPRESS};
	initSoundFX(); 
#else
	enableNonBlockingInput(); // desactiva ICANON mode
#endif

#ifdef RASPI
	playTitleScreenMusic();	
	drawTitleScreen();
	reverseClearDelay();
#endif
	askForName(&player);

	do // outer loop
	{
		initGame(&player);
		initMenu();		

		// timer	
		double fallInterval = getSpeed(0); // en seg (rapidez inicial nivel 0)
		double startTime, currentTime;
		startTime = getTime();

#ifdef RASPI
		double musicTimer = getTime();		
		startMusic();
#endif 

		do // inner loop
		{

#ifdef RASPI
			// musica			
			musicTimer = refreshMusic(musicTimer);


			// control input
			joystick = joy_read();
			key = whichKeyWasPressed(&joystick);
			performMove(&player, key);
#else
			if (kbhit()) // se pregunta si se presiono una tecla
			{
				key = getchar();
				performMove(&player, key); // puede cambiar el menuStatus a OPEN
			}
#endif
			// menu
			while (menuStatus() == OPEN) 
			{
#ifdef RASPI
				joystick = joy_read();
				key = whichKeyWasPressed(&joystick);
#else							
				key = getchar();
#endif				
				navigateMenu(key);
				printMenu();
			}		

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

			int previousLevel = player.level;
			player.level = player.lines / 10;

			if (previousLevel != player.level)
#ifdef RASPI
				playLevelUpSound();
#else 	
				;		
#endif
			player.score += howMuchScore(player.level, linesCombo);

			// rendering
			updateScene(&player);
#ifdef RASPI
			drawInRaspberry(&player);
#else
			drawInTerminal(&player);
#endif

			// updates speed	
			if (player.level < MAX_LEVEL)
				fallInterval = getSpeed(player.level);
		
			// delay
			usleep(20000); // = 0.02 seg. para que renderize suavemente

		} while (!isGameOver() && menuStatus() == RESUME);
	} while (menuStatus() != EXIT);

	// lista mejores puntajes
	updateTopScore("leaderboard.dat", player.score, player.name);
	printTopScores();

	// frees y finalizacioens
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
