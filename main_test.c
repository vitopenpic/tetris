#include "tetramino.h"
#include "board.h"
#include "game.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**************************************************************************/

int main(void)
{
	double fallInterval = 0.5; // en seg
	double startTime, currentTime;
	startTime = getTime();

	srand((unsigned int)time(NULL)); // seed para rand()

	player_t player;
	initGame(&player);

	enableNonBlockingInput(); // desactiva ICANON mode

	while (!isGameOver())
	{
		// control
		if (kbhit())
		{
			int key = getchar();
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
		//time_t currentTime = time(NULL);
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
		printf("\n");

		usleep(10000); // = 2 seg. no bajar de 2000 pq no renderea bn

		eraseLineIfFull();
	}
	
	puts("This is your end, cowboy...");
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
	return 0;
}
