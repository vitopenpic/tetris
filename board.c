/**************************************************************************
 * @file board.c
 * @authors
 * @brief
 **************************************************************************/

#include "tetramino.h"
#include "board.h"
#include <stdio.h>
#include <stdbool.h>

static bool mBoard[BOARD_HEIGHT][BOARD_WIDTH]; /* tablero con los tetraminos
muertos (donde se chequean las colisiones)*/

static bool mScene[BOARD_HEIGHT][BOARD_WIDTH]; /* matriz con los
bloques muertos y el tetramino que cae (la que al fin y al cabo se grafica)*/

bool isMovementLegal(int tipo, int rotacion, int x, int y)
{
    // i2, j2 iteran por mTertamino e i1, j1 iteran por mBoard
    for (int i1 = x, i2 = 0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
    {
        for (int j1 = y, j2 = 0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
        {
            // si el tetramino esta fuera del tablero
            if (i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1)
            {
                if (getBlockType(tipo, rotacion, i2, j2) != FREE)
                    return false;
            }
            // si el tetramino colisiono
            if (j1 >= 0)
            {
                if ((getBlockType(tipo, rotacion, i2, j2) != FREE) &&
                    (mBoard[j1][i1] == OCCUPIED))
                    return false;
            }
        }
    }
    return true;
}

void storePieceInBoard(player_t *plr)
{
    // guarda cada bloque del tetramino en el board
    // i2, j2 iteran por mTertamino e i1, j1 iteran por mBoard
    for (int i1 = plr->x, i2 = 0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
    {
        for (int j1 = plr->y, j2 = 0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
        {
            // guarda solo los bloques que noe stan vacios
            if (getBlockType(plr->tipo, plr->rotacion, i2, j2) != FREE)
                mBoard[j1][i1] = OCCUPIED;
        }
    }
}

void updateScene(player_t *plr)
{
    // copia mBoard a mScene
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            mScene[j][i] = mBoard[j][i];
        }
    }

    // copia el tetramino que cae a mScene
    // i1, j1 iteran por mScene; i2, j2 iteran por mTetramino
    for (int i1 = plr->x, i2 = 0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
    {
        for (int j1 = plr->y, j2 = 0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
        {
            if ((getBlockType(plr->tipo, plr->rotacion, i2, j2) == OCCUPIED)
                && j1 >= 0 && i1 >= 0)
                mScene[j1][i1] = OCCUPIED;
        }
    }
}

void drawScene()
{
    for (int j = 0; j < BOARD_HEIGHT; j++)
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
			// bordes            
			if (i == 0)
				printf("<!");		

			if (mScene[j][i] == FREE)
                printf(" . ");
            else
                printf(" # ");

			// bordes
			if(i == BOARD_WIDTH - 1)
				printf("!>");
        }
        printf("\n");
    }
	printf("<!========================!>");
}

void clearScene()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
            mScene[j][i] = FREE;
    }
}

static void eraseLine(int y)
{
    for (int j = y; j > 0; j--)
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            mBoard[j][i] = mBoard[j - 1][i];
        }
    }
}

void eraseLineIfFull()
{
    for (int j = 0; j < BOARD_HEIGHT; j++)
    {
        int i = 0;
        while (i < BOARD_WIDTH)
        {
            if (mBoard[j][i] != OCCUPIED)
                break;
            i++;
        }
        if (i == BOARD_WIDTH)
            eraseLine(j);
    }
}

bool isGameOver()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (mBoard[0][i])
            return true;
    }
}
