#include "tetramino.h"
#include "board.h"
#include "game.h"
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

    while (true)
    {
        time_t currentTime = time(NULL);
        double elapsedTime = difftime(currentTime, startTime) * 1000;

        if (elapsedTime >= fallInterval) //caida libre
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

        clearScene();
        updateScene(&player);
        drawScene();
		printf("\n");

        usleep(1000); // = 1 seg. no bajar de 1000 pq no renderea bn
    }
    return 0;
}
