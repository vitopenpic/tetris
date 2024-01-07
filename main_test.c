#include "tetramino.h"
#include "board.h"
#include "game.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
/**************************************************************************/

int main(void)
{
	int fallInterval = 1000; // 1 seg
	time_t startTime = time(NULL);

	srand((unsigned int)time(NULL)); // seed para rand()

	player_t player;
	initGame(&player);

	enableNonBlockingInput(); // desactiva ICANON mode

	do
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
		time_t currentTime = time(NULL);
		double elapsedTime = difftime(currentTime, startTime) * 1000;

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

		usleep(2000); // = 2 seg. no bajar de 2000 pq no renderea bn

		eraseLineIfFull();
	} while (!isGameOver());
	
	puts("This is your end, cowboy...");
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
	return 0;
}
