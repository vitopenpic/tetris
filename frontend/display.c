/**************************************************************************
 * @file display.c
 * @authors
 * @brief
 **************************************************************************/

#include "display.h"
#include "disdrv.h"
#include "../backend/board.h"
#include "../backend/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef RASPI

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
#define N_PIECE_DISP_POSX 10  
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

static void reverseClearDelay()
{
	dcoord_t p;	// hago esto pq no me deja pasar tipo 'disp_write({x, y}, ...)'	
	for (int y = 0; y <= DISP_MAX_Y; y++)
    {
        for (int x = 0; x <= DISP_MAX_X; x++)
        {
			p.x = x; p.y = y;            
			disp_write(p, D_OFF);
			disp_update();
			usleep(5000);
        }
    }
}
#endif
/*=======================
	FUNCIONES PUBLICAS
  =======================*/

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
	drawSceneRaspberry();  

	// proxima pieza, arriba a la derecha
	drawNextPieceRaspberry(player);
		
	// muestra el puntaje, abajo a la derecha 
	drawScoreRaspberry(player);
}	
#endif

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

#ifdef RASPI
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

void printIndexedLetter(int indx, int position) // position < MAX_CHAR
{
	switch (position)
	{
	case 0:
		drawLetterToDisp(FIRST_L_POSX, LETTER_POSY, indx);
		break;
	case 1:
		drawLetterToDisp(SECND_L_POSX, LETTER_POSY, indx);
		break;
	case 2:
		drawLetterToDisp(THIRD_L_POSX, LETTER_POSY, indx);
		break;
	case 3:
		drawLetterToDisp(FRTH_L_POSX, LETTER_POSY, indx);
		break;
	}
}
#endif


