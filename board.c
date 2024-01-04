/**************************************************************************
 * @file board.c                                                    
 * @authors 
 * @brief                                                              
 **************************************************************************/

#include "tetramino.h"
#include "board.h"
#include <stdio.h>
#include <stdbool.h>

static bool mBoard[BOARD_HEIGHT][BOARD_WIDTH]; /* tablero con los tetraminos muertos (donde se chequean las colisiones)*/

static bool mScene[BOARD_HEIGHT][BOARD_WIDTH]; /* matriz con los 
bloques muertos y el tetramino que cae (la que al fin y al cabo se grafica)*/

bool isMovementLegal(player_t *plr)
{
    // i2,j2 iteran por mTertamino e i1,j1 iteran por el mBoard
    for (int i1 = plr->x, i2 = 0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
    {
        for (int j1 = plr->y, j2 = 0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
        {
            // si el tetramino esta fuera del tablero
            if (i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1)
            {
                if (getBlockType(plr->tipo, plr->rotacion, i2, j2) != 0)
                    return false;
            }
            // si el tetramino colisiono
            if (j1 >= 0)
            {
                if( (getBlockType(plr->tipo, plr->rotacion, i2, j2) != 0) && 
                    (mBoard[j1][i1] == OCCUPIED) )
                    return false;
            }
        }
    }
    return true;
}


void storePieceInBoard(player_t *plr)
{
    // guarda cada bloque del tetramino en el board
    // i2,j2 iteran por mTertamino e i1,j1 iteran por el mBoard
    for(int i1=plr->x, i2=0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
    {
        for(int j1=plr->y, j2=0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
        {
            // guarda solo los bloques que noe stan vacios
            if(getBlockType(plr->tipo, plr->rotacion, i2, j2) != 0)
                mBoard[j1][i1] = OCCUPIED;
        }
    }
}

void updateScene(player_t *plr)
{
	// copia mBoard a mScene
	for(int i = 0; i < BOARD_WIDTH; i++)
		for(int j =0; j < BOARD_HEIGHT; j++)
			mScene[j][i] = mBoard [j][i];	

	// copia el tetramino que cae a mScene	
	// i1, j1 iteran por mScene; i2, j2 iteran por mTetramino
    for (int i1 = plr->x, i2 = 0; i2 < BLOCKS_PER_PIECE; i1++, i2++)
	{
		for (int j1 = plr->y, j2 = 0; j2 < BLOCKS_PER_PIECE; j1++, j2++)
		{
			if (getBlockType(plr->tipo, plr->rotacion, i2, j2) == OCCUPIED)
				mScene[j1][i1] = OCCUPIED;
		}
	}
}

void drawScene()
{
	for(int j=0; j < BOARD_HEIGHT; j++)
	{
		for(int i=0; i < BOARD_WIDTH; i++)
		{
			if(mScene[j][i] == FREE)
				printf(" - ");
			else
				printf(" * ");
		}
		printf("\n");
	}
}

void clearScene()
{
    for(int i=0; i < BOARD_WIDTH; i++)
    {
        for(int j=0; j < BOARD_HEIGHT; j++)
            mScene[j][i] = FREE;
    }
}






