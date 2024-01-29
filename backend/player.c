#include "player.h"
#include "board.h"
#include "menu.h"
#include "../frontend/soundFX.h"
#include "../frontend/control.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

	clearBoard();
	clearScene();
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

void performMove(player_t *player, char key)
{   
	switch (key)
    {
    case LEFT:
        if (isMovementLegal(player->tipo, player->rotacion,
                            player->x - 1, player->y))
        {
            player->x--;
        }
        break;
    case RIGHT:
        if (isMovementLegal(player->tipo, player->rotacion,
                            player->x + 1, player->y))
        {
            player->x++;
        }
        break;
    case DOWN:
        if (isMovementLegal(player->tipo, player->rotacion,
                            player->x, player->y + 1))
        {
            player->y++;
        }
        else
        {
            storePieceInBoard(player);
            createNewTetramino(player);
#ifdef RASPI
			playLockSound();
#endif
        }
        break;
    case ROTATE:
        if (isMovementLegal(player->tipo, (player->rotacion + 1) % 4,
                            player->x, player->y))
        {
            player->rotacion = (player->rotacion + 1) % 4;
        }
        break;
	case MENU: 
		setMenuStatusOpen();
		break;
    }
}



