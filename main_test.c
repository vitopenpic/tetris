#include "tetramino.h"
#include "board.h"
#include "game.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    int fallInterval = 1000; // 1 seg
    time_t startTime = time(NULL);
    
	srand((unsigned int)time(NULL)); // seed para rand()
    
	player_t player;
    initGame(&player);    

    clearScene();
    updateScene(&player);
    drawScene();

	enableNonBlockingInput(); // desactiva ICANON mode

    while (true)
    {
		// control
		if (kbhit()) 
		{
            int key = getchar();
            switch (key) 
			{
                case 'a':
                    player.x--;
                    if (!isMovementLegal(&player)) 
					{
                        player.x++;
                    }
                    break;
				case 'd':
                    player.x++;
                    if (!isMovementLegal(&player)) 
					{
                        player.x--;
                    }
                    break;
				case 's':
                    player.y++;
                    if (!isMovementLegal(&player)) 
					{
                        player.y--;
						storePieceInBoard(&player);
						createNewTetramino(&player);
					}
                    break;
				case 'w':
                    player.rotacion = (player.rotacion + 1) % 4;
                    if (!isMovementLegal(&player)) 
					{
                        player.rotacion = (player.rotacion - 1) % 4;
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

            player.y++;

		    if (!isMovementLegal(&player))
		    {
		        player.y--;
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
		if (isGameOver())
		{
			puts("This is your end, cowboy...");
			break;
		}
	}

	restoreBlockingInput(); /* el codigo no suele llegar hasta aca
	por esto escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
    return 0;
}
