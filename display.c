/**************************************************************************
 * @file display.c
 * @authors
 * @brief
 **************************************************************************/

#include "display.h"
#include "board.h"
#include "disdrv.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef RASPI

#define N_PIECE_DIMX 4
#define N_PIECE_DIMY 2
#define N_PIECE_DISP_POSX 10 // posicion donde se mostrara la proxima 
#define N_PIECE_DISP_POSY 1  // pieza en el display matricial

// arreglo de las piezas a mostrar
// en orden de: O, I, L, J, Z, S, T.
static const bool aNextPiece[7][N_PIECE_DIMY][N_PIECE_DIMX] = 
{
	{{0,1,1,0},
	 {0,1,1,0}},

	{{1,1,1,1},
	 {0,0,0,0}},

	{{0,0,0,1},
	 {0,1,1,1}},

	{{1,0,0,0},
	 {1,1,1,0}},

	{{1,1,0,0},
	 {0,1,1,0}},

	{{0,0,1,1},
	 {0,1,1,0}},

	{{0,0,1,0},
	 {0,1,1,1}}
};
#endif 

static void drawScene()
{
    for (int j = 0; j < BOARD_HEIGHT; j++)
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            // bordes
            if (i == 0)
                printf("<!");

            if (getScene(i, j) == FREE)
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

static void clearScreen()
{
    printf("\n\033[2J\033[H"); // limpia la pantalla
    // https://stackoverflow.com/questions/55672661/what-this-character-sequence-033h-033j-does-in-c
}


static void printNextPiece(player_t *plr)
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

void drawInTerminal(player_t *player)
{
	printf("LEVEL: %d\nSCORE: %d\nLINES: %d\n\n", 
	player->level, player->score, player->lines);
	printNextPiece(player);
	printf("\n");		
	drawScene();
	clearScreen();
}

#ifdef RASPI
void drawInRaspberry(player_t *player)
{
	// tablero, en la mitad izquierda del display 
	// copia mScene al display   
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (getScene(x,y) == OCCUPIED)
                disp_write({x, y}, D_ON);
            else
                disp_write({x, y}, D_OFF);
        }
    }
    disp_update();

	// proxima pieza y puntos/nivel en la otra mitad
	// proxima pieza
	for (int y1 = N_PIECE_DISP_POSY, y2 = 0; y2 < N_PIECE_DIMY; y1++, y2++)
	{
		for (int x1 = N_PIECE_DISP_POSX, x2 = 0; x2 < N_PIECE_DIMX; x1++, x2++)
		{
			if (aNextPiece[player->new_tipo][y2][x2] == OCCUPIED)
				disp_write({x1, y1}, D_ON);
			else
				disp_write({x1, y1}, D_OFF);
		}	
	}
	
}	
#endif




