#include "player.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static int getRandInBetween(int a, int b)
{
    if (a > b)
    {
        int aux = a;
        a = b;
        b = aux;
    }
    return rand() % (b - a + 1) + a;
}

static void clearInputBuffer() 
{
    fflush(stdin); 
}

static bool enterName(player_t *player)
{
	char c;
	int i = 0;
	while ((c = getchar()) != '\n' && i < MAX_CHAR - 1)
	{
		if (c < 'A' || c > 'z' || (c < 'a' && c > 'Z'))
		{
			puts("What is that sound!? Unspokeable! Please use enlgish letters...");
			return false;
		}		
		player->name[i] = c;
		i++;		
	}	
	player->name[i] = '\0';
	
	clearInputBuffer();
	return true;
}

static bool isThatThyName(player_t *player)
{
	printf("Outlandish it is! Thou really goes by the name of %s?\n[Y/N]\n", player->name);
	char c = getchar();
	while(getchar() != '\n');
	if (c == 'Y' || c == 'y')
	{
		printf("I hast never heard such name... cool\n");
		return true;
	}
	else if (c == 'N' || c == 'n') 	
	{
		printf("I beg yerr pardon. Tell me again, how thou art called?\n");
		return false;		
	}
	else
	{
		printf("Stop mumbling gibberish! Doth thou not speak english? Tell me how thou art called...\n");
		return false;
	}
}

void initGame(player_t *player)
{
    // primer tetramino
    player->tipo = getRandInBetween(0, 6);
    player->rotacion = getRandInBetween(0, 3);
    player->x = (BOARD_WIDTH / 2) + getInitPosX(player->tipo,
                                                player->rotacion);
    player->y = getInitPosY(player->tipo, player->rotacion);

    // proximo tetramino
    player->new_tipo = getRandInBetween(0, 6);
    player->new_rotacion = getRandInBetween(0, 3);

	player->lines = 0;
	player->score = 0;
	player->level = 0;

	printf("Heigh ho! Enter thy four character name...\n");
	do
	{
		while(!enterName(player));
	} while (!isThatThyName(player));
}

void createNewTetramino(player_t *player)
{
    // nuevo tetramino
    player->tipo = player->new_tipo;
    player->rotacion = player->new_rotacion;
    player->x = (BOARD_WIDTH / 2) + getInitPosX(player->tipo,
                                                player->rotacion);
    player->y = getInitPosY(player->tipo, player->rotacion);

    // nuevo proximo tetramino
    player->new_tipo = getRandInBetween(0, 6);
    player->new_rotacion = getRandInBetween(0, 3);
}





