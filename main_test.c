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

	player_t player;
	initGame(&player);

	enableNonBlockingInput(); // desactiva ICANON mode

	char key;
	
	do
	{
		// control
		if (kbhit()) // se pregunta si se presiono una tecla
		{
			key = getchar();
			switch (key)
			{
			case 'a':
				if (isMovementLegal(player.tipo, player.rotacion,
									player.x - 1, player.y))
				{
					player.x--;
				}
				break;
			case 'd':
				if (isMovementLegal(player.tipo, player.rotacion,
									player.x + 1, player.y))
				{
					player.x++;
				}
				break;
			case 's':
				if (isMovementLegal(player.tipo, player.rotacion,
									player.x, player.y + 1))
				{
					player.y++;
				}
				else
				{
					storePieceInBoard(&player);
					createNewTetramino(&player);
				}
				break;
			case 'w':
				if (isMovementLegal(player.tipo, (player.rotacion + 1) % 4,
									player.x, player.y))
				{
					player.rotacion = (player.rotacion + 1) % 4;
				}
				break;
			}
		}

		// caida libre
		currentTime = getTime();
		double elapsedTime = currentTime - startTime;

		if (elapsedTime >= fallInterval)
		{
			startTime = currentTime;

			if (isMovementLegal(player.tipo, player.rotacion,
								player.x, player.y + 1))
			{
				player.y++;
			}
			else
			{
				storePieceInBoard(&player);
				createNewTetramino(&player);
			}
		}

		// rendering
		updateScene(&player);
		drawScene();
		clearScreen();

		usleep(5000); // = 0.005 seg. para que renderee suavemente

		eraseLineIfFull();
	} while (!isGameOver() && key != 'x'); 
	
	if (key == 'x')
		puts("I guess 'tis our goodbye then. Godspeed to you, traveler...");
	else
		puts("This is your end, cowboy...");

	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
	return 0;
}
