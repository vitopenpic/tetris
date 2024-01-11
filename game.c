#include "game.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>


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

// https://harddrop.com/wiki/Scoring
int howMuchScore(int level,  int lines)
{
	if (lines == 1)
		return 4 * (level + 1);
	
	if (lines == 2)
		return 10 * (level + 1);

	if (lines == 3)
		return 30 * (level + 1);

	if (lines == 4)
		return 120 * (level + 1);

	else
		return 0;
}





