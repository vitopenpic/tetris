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
#elif ALLEGRO
	initAllegro();
	initSoundFX();
	drawTitle();
#else
	enableNonBlockingInput(); // desactiva ICANON mode
#endif
	do // espera a el nombre del jugador	
	{
		enterName(player.name);			//(espera input)
	} while (!confirmName(player.name)); 

	do // outer loop -------------------------------------------------------
	{
		initGame(&player);
		initMenu();

		// timer -----------------------------------------------------------
		double fallInterval = getSpeed(0) + SPEED_ADJUSTMENT; 
			// en seg (rapidez inicial nivel 0)
		double startTime, currentTime;
		startTime = getTime();
#ifdef RASPI
		disp_clear();
		disp_update();
#endif
		double musicTimer = getTime();
		startMusic();


		do 	// inner loop --------------------------------------------------
		{
			// musica ------------------------------------------------------			
#ifdef RASPI
			musicTimer = refreshMusic(musicTimer);
#endif

			// input control -----------------------------------------------
#ifdef RASPI
			joystick = joy_read();
			key = whichKeyWasPressed(&joystick);
			performMove(&player, key); // puede cambiar el menuStatus a OPEN
#elif ALLEGRO 
		    processKeyboardEvents(&player);
#else
			if (kbhit()) // se pregunta si se presiono una tecla
			{
				key = getchar();
				performMove(&player, key); // puede cambiar el menuStatus a OPEN
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
				if (prev_key == key) continue;
#elif ALLEGRO
				musicTimer = refreshMusic(musicTimer);
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
			int previousLevel = player.level;
			player.level = player.lines / 10;


			if (previousLevel != player.level)
				playLevelUpSound();

			player.score += howMuchScore(player.level, linesCombo);

			// rendering ------------------------------------------------------
			updateScene(&player);
#ifdef RASPI
			drawInRaspberry(&player);
#elif ALLEGRO
			drawInAllegro(&player);	
			//}		
#else		
			drawInTerminal(&player);
#endif

			// updates speed ---------------------------------------------------
			fallInterval = getSpeed(player.level) + SPEED_ADJUSTMENT; 
				// MAX_LEVEL es la rapidez max

			// delay -----------------------------------------------------------
			usleep(20000); // = 0.02 seg. para que renderize suavemente

		} while (!isGameOver() && menuStatus() == RESUME); // end of inner loop-
		// retry menu ----------------------------------------------------------
		if (isGameOver())
		{
#ifdef RASPI
			playGameOverSound();	
			reverseClearDelay();		
#endif
			if(!confirmRetry())
				wantToExit();
		}
	} while (menuStatus() != EXIT); // end of outer loop ------------------------
#ifdef RASPI
	stopMusic();
#endif

	// lista mejores puntajes ---------------------------------------------------
	updateLeaderboard("leaderboard.dat", player.score, player.level, player.name);
	printLeaderboard();

	// frees y finalizacioens ---------------------------------------------------
#ifdef RASPI
	endSoundFX();
	disp_clear();
	disp_update();
#elif ALLEGRO
	destroyAllegro();
#else
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
#endif
	return 0;
}
