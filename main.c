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
	// inicializaciones ---------------------------------------------------
	srand((unsigned int)time(NULL)); // seed para rand()
	player_t player;
	char key;
#ifdef RASPI
	joy_init();
	disp_init();
	joyinfo_t joystick = {0, 0, J_NOPRESS};
	initSoundFX();
	playTitleScreenMusic();
	drawTitleScreen();
#else
	enableNonBlockingInput(); // desactiva ICANON mode
#endif
	// espera a el nombre del jugador	
	do 
	{
#ifdef RASPI
		enterNameRasp(player.name, &joystick);
	} while (!confirmName(player.name));
#else
		enterName(player.name);			//(espera input)
	} while (!confirmName(player.name)); 
#endif

	do // outer loop -------------------------------------------------------
	{
		initGame(&player);
		initMenu();

		// timer -----------------------------------------------------------
		double fallInterval = getSpeed(0); // en seg (rapidez inicial nivel 0)
		double startTime, currentTime;
		startTime = getTime();
#ifdef RASPI
		disp_clear();
		double musicTimer = getTime();
		startMusic();
#endif

		do // inner loop ---------------------------------------------------
		{
#ifdef RASPI
			// musica ------------------------------------------------------
			musicTimer = refreshMusic(musicTimer);

			// input control -----------------------------------------------
			joystick = joy_read();
			key = whichKeyWasPressed(&joystick);
			performMove(&player, key); // puede cambiar el menuStatus a OPEN
#else
			if (kbhit()) // se pregunta si se presiono una tecla
			{
				key = getchar();
				performMove(&player, key);
			}
#endif
			// menu ---------------------------------------------------------
			while (menuStatus() == OPEN)
			{
#ifdef RASPI
				char prev_key = key;
				musicTimer = refreshMusic(musicTimer);
				joystick = joy_read();
				key = whichKeyWasPressed(&joystick);
				if (prev_key == key)
					continue;
#else
				key = getchar();
#endif
				navigateMenu(key);
				printMenu();
#ifdef RASPI
				if (key == ENTER)
					disp_clear();
#endif
			}

			// caida libre ---------------------------------------------------
			currentTime = getTime();
			double elapsedTime = currentTime - startTime;

			if (elapsedTime >= fallInterval)
			{
				startTime = currentTime;
				performMove(&player, DOWN);
			}

			// score/points & level ------------------------------------------
			int linesCombo = eraseLineIfFull();
			player.lines += linesCombo;
#ifdef RASPI
			int previousLevel = player.level;
#endif
			player.level = player.lines / 10;

#ifdef RASPI
			if (previousLevel != player.level)
				playLevelUpSound();
#endif
			player.score += howMuchScore(player.level, linesCombo);

			// rendering ------------------------------------------------------
			updateScene(&player);
#ifdef RASPI
			drawInRaspberry(&player);
#else
			drawInTerminal(&player);
#endif

			// updates speed ---------------------------------------------------
			if (player.level < MAX_LEVEL)
				fallInterval = getSpeed(player.level);

			// delay
			usleep(20000); // = 0.02 seg. para que renderize suavemente

		} while (!isGameOver() && menuStatus() == RESUME); // end of inner loop-
		// retry menu ----------------------------------------------------------
		if (isGameOver() && menuStatus() == RESUME)
		{
			do
			{
#ifdef RASPI
				musicTimer = refreshMusic(musicTimer);
#endif
				// #ifdef RASPI
				//				joystick = joy_read();
				//				key = whichKeyWasPressed(&joystick);
				// #else
				puts("\nWant to retry? [Y/N]");
				key = getchar();
				printf("%c\n", key);
				while (getchar() != '\n')
					;
				// #endif
			} while (key != 'Y' && key != 'y' && key != 'N' && key != 'n');
			wantToRetry(key);
		}
	} while (menuStatus() != EXIT); // end of outer loop ------------------------

	// lista mejores puntajes ---------------------------------------------------
	updateTopScore("leaderboard.dat", player.score, player.name);
	printTopScores();

	// frees y finalizacioens ---------------------------------------------------
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
