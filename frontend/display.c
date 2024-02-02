/**************************************************************************
 * @file display.c
 * @authors
 * @brief
 **************************************************************************/

#include "display.h"
#include "disdrv.h"
#include "control.h"
#include "../backend/board.h"
#include "../backend/menu.h"
#include "../backend/score.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef RASPI

/*================================
	   CONSTANTES PARA RASPI
  ================================*/

#define N_PIECE_DIMX 4
#define N_PIECE_DIMY 2

#define DISPLAY_NUMX 3
#define DISPLAY_NUMY 5

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

static const bool aDisplayNum[10][DISPLAY_NUMY][DISPLAY_NUMX] = 
{
	{{1,1,1}, // 0
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},

	{{0,0,1}, // 1
	 {0,0,1},
	 {0,0,1},
	 {0,0,1},
	 {0,0,1}},

	{{1,1,1}, // 2
	 {0,0,1},
	 {1,1,1},
	 {1,0,0},
	 {1,1,1}},

	{{1,1,1}, // 3
	 {0,0,1},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}},

	{{1,0,1}, // 4
	 {1,0,1},
	 {1,1,1},
	 {0,0,1},
	 {0,0,1}},

	{{1,1,1}, // 5
	 {1,0,0},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}},

	{{1,1,1}, // 6
	 {1,0,0},
	 {1,1,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,1,1}, // 7
	 {0,0,1},
	 {0,0,1},
	 {0,0,1},
	 {0,0,1}},

	{{1,1,1}, // 8
	 {1,0,1},
	 {1,1,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,1,1}, // 9
	 {1,0,1},
	 {1,1,1},
	 {0,0,1},
	 {0,0,1}},
};

#define ALPHA_MAX 26
static const bool aDisplayAlphabet[ALPHA_MAX][DISPLAY_NUMY][DISPLAY_NUMX] = 
{
	{{0,1,0}, // A
	 {1,0,1},
	 {1,1,1},
	 {1,0,1},
	 {1,0,1}},

	{{1,1,1}, // B
	 {1,0,1},
	 {1,1,0},
	 {1,0,1},
	 {1,1,1}},

	{{1,1,1}, // C
	 {1,0,0},
	 {1,0,0},
	 {1,0,0},
	 {1,1,1}},

	{{1,1,0}, // D
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,0}},
	 
	{{1,1,1}, // E
	 {1,0,0},
	 {1,1,0},
	 {1,0,0},
	 {1,1,1}},

	{{1,1,1}, // F
	 {1,0,0},
	 {1,1,0},
	 {1,0,0},
	 {1,0,0}},

	{{1,1,1}, // G
	 {1,0,0},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,0,1}, // H
	 {1,0,1},
	 {1,1,1},
	 {1,0,1},
	 {1,0,1}},

	{{1,1,1}, // I
	 {0,1,0},
	 {0,1,0},
	 {0,1,0},
	 {1,1,1}},

	{{0,0,1}, // J
	 {0,0,1},
	 {0,0,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,0,1}, // K
	 {1,0,1},
	 {1,1,0},
	 {1,0,1},
	 {1,0,1}},

	{{1,0,0}, // L
	 {1,0,0},
	 {1,0,0},
	 {1,0,0},
	 {1,1,1}},

	{{1,1,1}, // M
	 {1,1,1},
	 {1,0,1},
	 {1,0,1},
	 {1,0,1}},

	{{1,1,1}, // N
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,0,1}},

	{{1,1,1}, // O
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,1,1}, // P
	 {1,0,1},
	 {1,1,1},
	 {1,0,0},
	 {1,0,0}},

	{{1,1,1}, // Q
	 {1,0,1},
	 {1,0,1},
	 {1,1,1},
	 {1,1,1}},

	{{1,1,1}, // R
	 {1,0,1},
	 {1,1,0},
	 {1,0,1},
	 {1,0,1}},

	{{1,1,1}, // S
	 {1,0,0},
	 {1,1,1},
	 {0,0,1},
	 {1,1,1}},

	{{1,1,1}, // T
	 {0,1,0},
	 {0,1,0},
	 {0,1,0},
	 {0,1,0}},

	{{1,0,1}, // U
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {1,1,1}},

	{{1,0,1}, // V
	 {1,0,1},
	 {1,0,1},
	 {1,0,1},
	 {0,1,0}},

	{{1,0,1}, // W
	 {1,0,1},
	 {1,0,1},
	 {1,1,1},
	 {1,1,1}},

	{{1,0,1}, // X
	 {1,0,1},
	 {0,1,0},
	 {1,0,1},
	 {1,0,1}},

	{{1,0,1}, // Y
	 {1,0,1},
	 {1,1,1},
	 {0,1,0},
	 {0,1,0}},

	{{1,1,1}, // Z
	 {0,0,1},
	 {0,1,0},
	 {1,0,0},
	 {1,1,1}},
};

const static bool mTitleScreen[DISP_MAX_X+1][DISP_MAX_Y+1] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0},
	{0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0},
	{0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0},
	{0,1,0,1,0,1,0,0,0,1,0,1,0,0,1,0},
	{0,1,0,1,0,1,1,1,0,0,0,1,0,1,1,0},
	{0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,0},
	{0,1,1,1,0,1,1,1,1,1,0,1,1,0,1,0},
	{0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,0},
	{0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
	{0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0},
	{0,1,0,1,0,0,0,1,0,0,0,0,0,1,1,0},
	{0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,0},
	{0,1,0,1,0,0,0,1,0,0,0,0,0,1,1,0},
	{0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

const static bool mMenuScreens[3][DISP_MAX_Y+1][DISP_MAX_X+1] =
{
	{ // RESUME (RSME)
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
		{0,1,0,1,0,1,0,0,0,1,1,1,0,1,0,0},
		{0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0},
		{0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0},
		{0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{ // RESTART (RSTT)
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
		{0,1,0,1,0,1,0,0,0,0,1,0,0,0,1,0},
		{0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0},
		{0,1,0,1,0,0,0,1,0,0,1,0,0,0,1,0},
		{0,1,0,1,0,1,1,1,0,0,1,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{ // EXIT	
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,1,0,1,0,0,1,0,0,1,1,1},
		{0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
		{0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0},
		{0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0},
		{0,1,1,1,0,1,0,1,0,0,1,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},
};

const static bool mNameScreen[DISP_MAX_Y+1][DISP_MAX_X+1] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0,1,0,0,1,1,1,0,0},
	{0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,0},
	{0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,0},
	{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0},
	{0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

#define YES_NO_DIMX 14
#define YES_NO_DIMY 9
const static bool mYesOrNo[2][YES_NO_DIMY][YES_NO_DIMX] =
{
	{
		{0,0,0,0,0,0,0,1,1,1,1,1,1,1},
		{0,0,1,0,1,0,0,1,1,0,0,0,1,1},
		{0,0,1,0,1,0,0,1,1,0,1,0,1,1},
		{0,0,1,1,1,0,0,1,1,0,1,0,1,1},
		{0,0,0,1,0,0,0,1,1,0,1,0,1,1},
		{0,0,0,1,0,0,0,1,1,0,1,0,1,1},
		{0,0,0,0,0,0,0,1,1,1,1,1,1,1}
	},
	{
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0},
		{1,1,0,1,0,1,1,0,0,1,1,1,0,0},
		{1,1,0,1,0,1,1,0,0,1,0,1,0,0},
		{1,1,0,0,0,1,1,0,0,1,0,1,0,0},
		{1,1,1,0,1,1,1,0,0,1,0,1,0,0},
		{1,1,1,0,1,1,1,0,0,1,0,1,0,0},
		{1,1,1,1,1,1,1,0,0,0,0,0,0,0}
	}
};

#endif 

/*================================
	FUNCIONES AUXILIARES TERMINAL
  ================================*/

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

/*================================
	FUNCIONES AUXILIARES RASPi
  ================================*/

#ifdef RASPI

/* posicion con respecto al display de la raspi donde se mostrara 
la proxima pieza */
#define N_PIECE_DISP_POSX 9  
#define N_PIECE_DISP_POSY 1  

/* posicion de los numeros del puntaje con respecto al display de
la raspi*/
#define SCORE_THOUSAND_X 9
#define SCORE_THOUSAND_Y 4
#define SCORE_HUNDRED_X 13
#define SCORE_HUNDRED_Y 4
#define SCORE_TEN_X 9
#define SCORE_TEN_Y 10
#define SCORE_UNIT_X 13
#define SCORE_UNIT_Y 10

static void drawSceneRaspberry()
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
			p.x = x; p.y = y;            
			if (getScene(x,y) == OCCUPIED)
                disp_write(p, D_ON);
            else
                disp_write(p, D_OFF);
        }
    }
    disp_update();
}

static void drawNextPieceRaspberry(player_t *player)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y1 = N_PIECE_DISP_POSY, y2 = 0; y2 < N_PIECE_DIMY; y1++, y2++)
	{
		for (int x1 = N_PIECE_DISP_POSX, x2 = 0; x2 < N_PIECE_DIMX; x1++, x2++)
		{
			p.x = x1; p.y = y1;			
			if (aNextPiece[player->new_tipo][y2][x2] == OCCUPIED)
				disp_write(p, D_ON);
			else
				disp_write(p, D_OFF);
		}	
	}
}

#define BIT0_LVL_POSX 14
#define BIT0_LVL_POSY 0

#define BIT1_LVL_POSX 15
#define BIT1_LVL_POSY 0

#define BIT2_LVL_POSX 14
#define BIT2_LVL_POSY 1

#define BIT3_LVL_POSX 15
#define BIT3_LVL_POSY 1

#define BIT4_LVL_POSX 14
#define BIT4_LVL_POSY 2 

#define BIT5_LVL_POSX 15
#define BIT5_LVL_POSY 2

static const int aScoreBit[6][2] = {
	{BIT0_LVL_POSX, BIT0_LVL_POSY}, {BIT1_LVL_POSX, BIT1_LVL_POSY},
	{BIT2_LVL_POSX, BIT2_LVL_POSY}, {BIT3_LVL_POSX, BIT3_LVL_POSY}, 
	{BIT4_LVL_POSX, BIT4_LVL_POSY}, {BIT5_LVL_POSX, BIT5_LVL_POSY}   
};

static void drawLevelRaspberry(int level)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'
	unsigned int mask = 0b000001;
	for (int i = 0; i < 6; i++)
	{
		p.x = aScoreBit[i][0]; p.y = aScoreBit[i][1];
		if (level & (mask << i)) 
			disp_write(p, D_ON);
		else 
			disp_write(p, D_OFF);
	}	
}

static void drawNumberToDisp(int x0, int y0, int num)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y1 = y0, y2 = 0; y2 < DISPLAY_NUMY; y1++, y2++)
	{
		for (int x1 = x0, x2 = 0; x2 < DISPLAY_NUMX; x1++, x2++)
		{
			p.x = x1; p.y = y1;
			if (aDisplayNum[num][y2][x2] == OCCUPIED)
				disp_write(p, D_ON);
			else 	
				disp_write(p, D_OFF);			
		}
	}
}

static void drawLetterToDisp(int x0, int y0, int lett)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y1 = y0, y2 = 0; y2 < DISPLAY_NUMY; y1++, y2++)
	{
		for (int x1 = x0, x2 = 0; x2 < DISPLAY_NUMX; x1++, x2++)
		{
			p.x = x1; p.y = y1;
			if (aDisplayAlphabet[lett][y2][x2] == OCCUPIED)
				disp_write(p, D_ON);
			else 	
				disp_write(p, D_OFF);			
		}
	}
	disp_update();
}

static void drawMenuStatus(menu_status_t status)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y <= DISP_MAX_Y; y++)
    {
        for (int x = 0; x <= DISP_MAX_X; x++)
        {
			p.x = x; p.y = y;            
			if (mMenuScreens[status][y][x] == OCCUPIED)
                disp_write(p, D_ON);
            else
                disp_write(p, D_OFF);
        }
    }
}

static void	drawScoreRaspberry(player_t *player)
{
	if (player->score > 9999)
	{
		drawNumberToDisp(SCORE_UNIT_X, SCORE_UNIT_Y, 9);  
		drawNumberToDisp(SCORE_TEN_X, SCORE_TEN_Y, 9);
		drawNumberToDisp(SCORE_HUNDRED_X, SCORE_HUNDRED_Y, 9);
		drawNumberToDisp(SCORE_THOUSAND_X, SCORE_THOUSAND_Y, 9);
	}	

	// muestra la unidad del puntaje	 
	drawNumberToDisp(SCORE_UNIT_X, SCORE_UNIT_Y, player->score % 10);  
	// muestra la decena del puntaje
	drawNumberToDisp(SCORE_TEN_X, SCORE_TEN_Y, player->score % 100 / 10);
	// muestra la centena del puntaje
	drawNumberToDisp(SCORE_HUNDRED_X, SCORE_HUNDRED_Y, player->score % 1000 / 100);
	// muestra el millar del puntaje
	drawNumberToDisp(SCORE_THOUSAND_X, SCORE_THOUSAND_Y, player->score % 10000 / 1000);	
}

static void drawPosition(int n)
{
	dcoord_t p;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			p.x = i; p.y = j;
			disp_write(p, D_ON);
		} 
	}
}

static void drawLeaderboardRaspi()
{
	int i = 0, prev_i = 0;
	do
	{
		if (getLeaderboard(prev_i)->name[0] == 0) // no hay datos cargados
			continue;
		i = prev_i; // se cambia el indice
		disp_clear();
		printString2Rasp(getLeaderboard(i)->name, 4);
		// muestra el millar del puntaje
		drawNumberToDisp(1, 11, getLeaderboard(i)->score % 10000 / 1000);
		// muestra la centena del puntaje
		drawNumberToDisp(5, 11, getLeaderboard(i)->score % 1000 / 100);
		// muestra la decena del puntaje
		drawNumberToDisp(9, 11, getLeaderboard(i)->score % 100 / 10);
		// muestra la unidad del puntaje	 
		drawNumberToDisp(13, 11, getLeaderboard(i)->score % 10);
		
		drawLevelRaspberry(getLeaderboard(i)->level);

		drawPosition(i + 1);
		disp_update();
	} while ((prev_i = indexOfLboard(i)) != -1);
}
#endif
/*================================
	FUNCIONES PUBLICAS TERIMAL
  ================================*/

void drawInTerminal(player_t *player)
{
	printf("LEVEL: %d\nSCORE: %d\nLINES: %d\n\n", 
	player->level, player->score, player->lines);
	printNextPiece(player);
	printf("\n");		
	drawScene();
	clearScreen();
}

/*================================
	 FUNCIONES PUBLICAS RASPI
  ================================*/

#ifdef RASPI
void drawInRaspberry(player_t *player)
{
	// tablero, en la mitad izquierda del display 
	drawSceneRaspberry();  

	// proxima pieza, arriba a la derecha
	drawNextPieceRaspberry(player);
		
	// muestra el puntaje, abajo a la derecha 
	drawScoreRaspberry(player);

	// muestra el nivel actual de forma binaria arriba al lado de nueva pieza
	drawLevelRaspberry(player->level);
}	

void reverseClearDelay()
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y <= DISP_MAX_Y; y++)
    {
        for (int x = 0; x <= DISP_MAX_X; x++)
        {
			p.x = x; p.y = y;            
			disp_write(p, D_OFF);
			disp_update();
			usleep(2000);
        }
    }
}

void drawTitleScreen()
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y <= DISP_MAX_Y; y++)
    {
        for (int x = 0; x <= DISP_MAX_X; x++)
        {
			p.x = x; p.y = y;            
			if (mTitleScreen[y][x] == OCCUPIED)
                disp_write(p, D_ON);
            else
                disp_write(p, D_OFF);
			disp_update();
			usleep(5000);
        }
    }
	reverseClearDelay();
}

void printNameScreen()
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y <= DISP_MAX_Y; y++)
    {
        for (int x = 0; x <= DISP_MAX_X; x++)
        {
			p.x = x; p.y = y;            
			if (mNameScreen[y][x] == OCCUPIED)
                disp_write(p, D_ON);
            else
                disp_write(p, D_OFF);
        }
    }
	disp_update();
}

#define LETTER_POSY 9
#define FIRST_L_POSX 1
#define SECND_L_POSX 5
#define THIRD_L_POSX 9
#define FRTH_L_POSX 13

const int letterPos[4] = {FIRST_L_POSX, SECND_L_POSX, 
THIRD_L_POSX, FRTH_L_POSX};

void printIndexedLetter(int indx, int position) // position < MAX_CHAR
{
	drawLetterToDisp(letterPos[position], LETTER_POSY, indx);
}

void printString2Rasp(char *string, int height)
{
	for(int i = 0; i < MAX_CHAR - 1; i++)
	{
		if (string[i] == '\0') break;
		drawLetterToDisp(letterPos[i], height, string[i] - 'A');
	}
	disp_update();
}

#define YES_NO_POSX 1
#define YES_NO_POSY 7
void printYesOrNo(bool state)
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y1 = YES_NO_POSY, y2 = 0; y2 < YES_NO_DIMY; y1++, y2++)
	{
		for (int x1 = YES_NO_POSX, x2 = 0; x2 < YES_NO_DIMX; x1++, x2++)
		{
			p.x = x1; p.y = y1;			
			if (mYesOrNo[state][y2][x2] == OCCUPIED)
				disp_write(p, D_ON);
			else
				disp_write(p, D_OFF);
		}	
	}
	disp_update();
}

#endif

/*===================================================
	 FUNCIONES PUBLICAS CON COMPILACION CONDICIONAL
  ===================================================*/

void printMenu()
{
#ifdef RASPI
	disp_clear();
	drawMenuStatus(abs(menuIndex()));
	disp_update();
#else
	clearScreen();	
	switch (abs(menuIndex()))
	{
		case RESUME:
			puts(">RESUME\n RESTART\n EXIT");
		break;
		case RESTART:
			puts(" RESUME\n>RESTART\n EXIT");
		break;
		case EXIT:
			puts(" RESUME\n RESTART\n>EXIT");
		break;
	}
#endif
}

void printLeaderboard(void)
{
#ifdef RASPI
	drawLeaderboardRaspi();
#else
	puts("\nThe legendary scroll of the best block stackers of all time:\n");	
	puts("   NAME  SCORE  LVL");
	for (int i = 0; i < MAX_SCORERS; i++)
	{
		if (getLeaderboard(i)->name[0] == '\0') // no hay nombre
			break;
		printf("%d  %s  %d    %d\n", i + 1, getLeaderboard(i)->name,
			   getLeaderboard(i)->score, getLeaderboard(i)->level);		
	}
#endif
}



