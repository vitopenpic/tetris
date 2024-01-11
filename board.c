/**************************************************************************
 * @file board.c
 * @authors
 * @brief
 **************************************************************************/

#include "tetramino.h"
#include "board.h"
#include "disdrv.h"
#include <stdio.h>
#include <stdbool.h>

static bool mBoard[BOARD_HEIGHT][BOARD_WIDTH]; /* tablero con los tetraminos
muertos (donde se chequean las colisiones)*/

static bool mScene[BOARD_HEIGHT][BOARD_WIDTH]; /* matriz con los
bloques muertos y el tetramino que cae (la que al fin y al cabo se grafica)*/

/* info: https://meatfighter.com/nintendotetrisai/#Lines_and_Statistics ,
seccion: "Dropping Tetrominos"

look-up-table con las rapideces del tetramino dependiendo del nivel, luego
del nivel 29, la rapidez no varia*/
const double aSpeed[MAX_LEVEL] = 
{0.8, 0.715, 0.632, 0.550, 0.466, 0.383, 0.300, 0.216, 0.133, 0.100, 0.083, 
 0.083, 0.083, 0.067, 0.067, 0.067, 0.050, 0.050, 0.050, 0.033, 0.033, 0.033,
 0.033, 0.033, 0.033, 0.033, 0.033, 0.033, 0.017}; 

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
            if ((getBlockType(plr->tipo, plr->rotacion, i2, j2) == OCCUPIED) && j1 >= 0 && i1 >= 0)
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
            if (i == BOARD_WIDTH - 1)
                printf("!>");
        }
        printf("\n");
    }
    printf("<!==============================!>");
}

void clearScene()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
            mScene[j][i] = FREE;
    }
}

#ifdef RASPI
void drawInDisplay()
{
    dcoord_t p;
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            p.x = x;
            p.y = y;
            if (mScene[y][x] == OCCUPIED)
                disp_write(p, D_ON);
            else
                disp_write(p, D_OFF);
        }
    }
    disp_update();
}
#endif

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

int eraseLineIfFull()
{
    int lines_erased = 0;
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
		{
            eraseLine(j);
			lines_erased++;
		}
    }
	return lines_erased;
}

bool isGameOver()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (mBoard[0][i])
            return true;
    }
    return false;
}

void clearScreen()
{
    printf("\n\033[2J\033[H"); // limpia la pantalla
    // https://stackoverflow.com/questions/55672661/what-this-character-sequence-033h-033j-does-in-c
}

void printNextPiece(player_t *plr)
{
	for (int j = 0; j < BLOCKS_PER_PIECE; j++)
	{
		for (int i = 0; i < BLOCKS_PER_PIECE; i++)
		{
			if (getBlockType(plr->new_tipo, plr->new_rotacion, i, j))
				printf(" # ");
			else
				printf("   ");
		}
	printf("\n");	
	}		
}

double getSpeed(int level)
{
	return aSpeed[level];
}
