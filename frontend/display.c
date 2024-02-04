/**************************************************************************
 * @file display.c
 * @authors
 * @brief
 **************************************************************************/

#include "display.h"
#include "disdrv.h"
#include "../backend/board.h"
#include "../backend/menu.h"
#include "../backend/tetramino.h"
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

/*==================================
	FUNCIONES AUXILIARES ALLEGRO
  ==================================*/

#ifdef ALLEGRO

#define ANCHO 800
#define ALTO 800
#define TAMBLOQUE 35
#define ANCHOTABLERO 10
#define ALTOTABLERO 20


#define N_PIECE_DIMX 4
#define N_PIECE_DIMY 4
#define N_PIECE_DISP_POSX 600
#define N_PIECE_DISP_POSY 100
#define BLOCK_SIZE 35

#define MAX_TETRAMINOS 7
#define MAX_ROTATIONS 4
#define TETRAMINO_DIM 4




//score level name
#define SCTXTX 370
#define SCTXTY 10
#define SCNMBX 370
#define SCNMBY 20


#define LVLTXTX 370
#define LVLTXTY 30
#define LVLNMBX 370
#define LVLNMBY 40



#define NMTXTX 370
#define NMTXTY 50
#define NMUSERX 370
#define NMUSERY 60

extern const bool mTetramino[MAX_TETRAMINOS][MAX_ROTATIONS][TETRAMINO_DIM][TETRAMINO_DIM];



// Declaración de colores
ALLEGRO_COLOR colors[8];



/*
static void drawNextPieceAllegro(player_t *player)
{
	 

    int piece = player->new_tipo;
    ALLEGRO_COLOR color;
    switch(piece) {

        case 0: color = al_map_rgb(255, 0, 0); break; // Rojo
        case 1: color = al_map_rgb(0, 255, 0); break; // Verde
        case 2: color = al_map_rgb(0, 0, 255); break; // Azul
        case 3: color = al_map_rgb(255, 255, 0); break; // Amarillo
        case 4: color = al_map_rgb(255, 0, 255); break; // Magenta
        case 5: color = al_map_rgb(0, 255, 255); break; // Cian
        case 6: color = al_map_rgb(255, 165, 0); break; // Naranja
    }

    // Limpiar la pieza anteriormente mostrada
    for (int y = 0; y < N_PIECE_DIMY; y++) {
        for (int x = 0; x < N_PIECE_DIMX; x++) {
            if (mTetramino[piece][0][y][x]) { // Mira la rotación 0 de cada pieza
                al_draw_filled_rectangle(N_PIECE_DISP_POSX + x * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSY + y * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSX + (x + 1) * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSY + (y + 1) * BLOCK_SIZE, 
                                         al_map_rgb(0, 0, 0)); // Limpiar con color negro
            }
        }
    }

    // Dibujar la nueva pieza
    for (int y = 0; y < N_PIECE_DIMY; y++) {
        for (int x = 0; x < N_PIECE_DIMX; x++) {
            if (mTetramino[piece][0][y][x]) { // Mira la rotación 0 de cada pieza
                al_draw_filled_rectangle(N_PIECE_DISP_POSX + x * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSY + y * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSX + (x + 1) * BLOCK_SIZE, 
                                         N_PIECE_DISP_POSY + (y + 1) * BLOCK_SIZE, 
                                         color);
            }
        }
    }

	
}

*/
void drawSceneAllegro()
{

	 // Definir el color del texto
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255); // Por ejemplo, blanco


    // Limpiar la pantalla con un color negro
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Dibujar la cuadrícula del tablero         (POSTERIOR A INGRESAR EL NOMBRE)
    for (int y = 0; y < ALTOTABLERO; y++)
	{
        for (int x = 0; x < ANCHOTABLERO; x++)
		{
            // Dibujar un rectángulo blanco para cada celda de la cuadrícula del tablero
            al_draw_rectangle(x * TAMBLOQUE, y * TAMBLOQUE,
                              (x + 1) * TAMBLOQUE, (y + 1) * TAMBLOQUE,
                              al_map_rgb(255, 255, 255), 1); // Blanco
        }
    }


	//Dibujar score level name
	alleTextinit (textColor);


    // Dibujar los bloques ocupados en la matriz de escena
    for (int y = 0; y < ALTOTABLERO; y++)
	{
        for (int x = 0; x < ANCHOTABLERO; x++)
		{
            if (getScene(x, y)) 
			{
                // Dibujar un rectángulo rojo en la posición (x, y)
                al_draw_filled_rectangle(x * TAMBLOQUE, y * TAMBLOQUE,
                                         (x + 1) * TAMBLOQUE, (y + 1) * TAMBLOQUE,
                                         al_map_rgb(255, 0, 0)); // Rojo
            }
        }
    }

    // Actualizar la ventana
	al_rest(0.02);
    al_flip_display();
}







// Funcion para mostrar un numero en pantalla con allegro
void drawNumber(int number, float x, float y, ALLEGRO_COLOR textColor)
{
    
    char numberString[20];
    snprintf(numberString, sizeof(numberString), "%d", number);

    
    al_draw_text(al_create_builtin_font(), textColor, x, y, ALLEGRO_ALIGN_LEFT, numberString);
}


// Funcion para mostrar uun texto en pantalla con allegro
void drawString(const char *text, float x, float y, ALLEGRO_COLOR textColor) 
{
    
    al_draw_text(al_create_builtin_font(), textColor, x, y, ALLEGRO_ALIGN_LEFT, text);
}




void alledrawinfo (player_t player, ALLEGRO_COLOR textColor)
{
	
	 drawNumber(player.score, SCNMBX, SCNMBY, textColor);
	 drawNumber(player.level, LVLNMBX, LVLNMBY,textColor );
	drawString(player.name, NMUSERX, NMUSERY,textColor );
}

void alleTextinit (ALLEGRO_COLOR textColor)
{
	
	al_draw_text(al_create_builtin_font(), textColor, SCTXTX, SCTXTY, ALLEGRO_ALIGN_LEFT, "SCORE:");
	al_draw_text(al_create_builtin_font(), textColor, LVLTXTX, LVLTXTY, ALLEGRO_ALIGN_LEFT, "LEVEL:");
	al_draw_text(al_create_builtin_font(), textColor, NMTXTX, NMTXTY, ALLEGRO_ALIGN_LEFT, "NAME:");
	
	
}
	
// Función para dibujar el título "TETRIS" centrado en la ventana
void drawTitle() 
{
    // Calcula la posición x y y para que el título esté centrado
    int x = (ANCHO - al_get_text_width(al_create_builtin_font(), "TETRIS")) / 2;
    int y = ALTO / 2; // Puedes ajustar esta posición según sea necesario

    // Dibuja el texto "TETRIS" con el carácter "0" en la posición calculada
    al_draw_text(al_create_builtin_font(), al_map_rgb(255,0 ,0 ), x, y, ALLEGRO_ALIGN_LEFT, "TETRIS");

	 // inserte su nombre
    al_draw_text(al_create_builtin_font(), al_map_rgb(255,255 ,255 ), x, y+20, ALLEGRO_ALIGN_LEFT, "Inserte su nombre");


	al_rest(0.02);
    al_flip_display();
}

#endif


/*==================================
	FUNCIONES PUBLICAS ALLEGRO
  ==================================*/

#ifdef ALLEGRO



// Dibujar la cuadrícula vacía

void dibutablero()
{
    // Dibujar la grilla del tablero
    for (int y = 0; y < BOARD_HEIGHT; y++)
 	{
        for (int x = 0; x < BOARD_WIDTH; x++)
	 	{
            al_draw_rectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, 
                              (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE, 
                              al_map_rgb(255, 255, 255), 1); // Blanco
        }
    }
}



void drawInAllegro(player_t *player)
{
	// Definir el color del texto
    ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255); // Por ejemplo, blanco
	
	al_rest(0.05);

	// tablero, en la mitad izquierda del display 
	drawSceneAllegro();  

	// proxima pieza, arriba a la derecha
	//drawNextPieceAllegro(player);
		
	 alledrawinfo(*player, textColor);

	// Actualizar la ventana
    al_flip_display();
	
   
}

void initAllegro() 
{
    // Inicializa Allegro		
    if (!al_init()) 
	{
    	fprintf(stderr, "Failed to initialize Allegro!\n");
        exit(-1);
    }
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();

    // Crear la cola de eventos (TECLADO)
	extern ALLEGRO_EVENT_QUEUE *event_queue;
	extern ALLEGRO_DISPLAY *display;
	

	

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

	
    // Inicializar colores
	extern ALLEGRO_COLOR colors[];
    colors[0] = al_map_rgb(0, 0, 0);      // Negro
    colors[1] = al_map_rgb(255, 0, 0);    // Rojo
    colors[2] = al_map_rgb(0, 255, 0);    // Verde
    colors[3] = al_map_rgb(0, 0, 255);    // Azul
    colors[4] = al_map_rgb(255, 255, 0);  // Amarillo
    colors[5] = al_map_rgb(255, 165, 0);  // Naranja
    colors[6] = al_map_rgb(128, 0, 128);  // Morado
    colors[7] = al_map_rgb(0, 255, 255);  // Cyan

    // Crea la ventana  
    display = al_create_display(800, 800);
    al_set_window_title(display, "Tetris");
}


void processKeyboardEvents(ALLEGRO_EVENT_QUEUE *event_queue, player_t *player)
{
    static double lastKeyPressTime = 0.0; // Variable para almacenar el tiempo de la última pulsación de tecla
    double currentTime = al_get_time(); // Obtener el tiempo actual

    ALLEGRO_EVENT event;
    while (al_get_next_event(event_queue, &event)) 
    {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
        {
            // Calcular el tiempo transcurrido desde la última pulsación
            double elapsedTime = currentTime - lastKeyPressTime;

            // Si ha pasado un tiempo suficiente desde la última pulsación, procesar la tecla actual
            if (elapsedTime > 0.1) { // Por ejemplo, si han pasado más de 0.1 segundos
                switch (event.keyboard.keycode) 
                {
                    case ALLEGRO_KEY_LEFT:
                        performMove(player, LEFT);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        performMove(player, RIGHT);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        performMove(player, DOWN);
                        break;
                    case ALLEGRO_KEY_UP:
                        performMove(player, ROTATE);
                        break;
                    default:
                        break;
                }

                // Actualizar el tiempo de la última pulsación de tecla
                lastKeyPressTime = currentTime;
            }
        }
    }
}



#endif



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



