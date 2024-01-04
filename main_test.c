#include "tetramino.h"
#include "board.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    //int fallInterval = 1000; // 1 seg
    //time_t startTime = time(NULL);
	srand((unsigned int)time(NULL)); // seed para rand() 

    player_t player;
    initGame(&player);

	int c;
	
	clearScene();
	updateScene(&player);
	drawScene();

    while ((c=getchar()) != 'x')
    {
        //time_t currentTime = time(NULL);
        //double elapsedTime = difftime(currentTime, startTime) * 1000;

        /*if (elapsedTime >= fallInterval)
        {
            startTime = currentTime;

            player.y++;
        }*/

		while((c=getchar()) != '\n');

		player.y++; // caida libre
		if (!isMovementLegal(&player))
		{
			player.y--;			
			storePieceInBoard(&player);
			createNewTetramino(&player);
		}
			
		clearScene();
		updateScene(&player);
		drawScene();

        usleep(1000);
    }
    return 0;
}

