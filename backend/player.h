/**************************************************************************
 * @file player.h
 * @authors
 * @brief archivo de cabecera que se encarga de la inicializacion del 
 * jugador, su puntaje, nivel y la creacion de las piezas
 **************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

/***********************************
 *	enums, defines, structs
 ***********************************/

#define MENU 'x'
#define LEFT 'a'
#define RIGHT 'd'
#define DOWN 's'
#define ROTATE 'w'

#define MAX_CHAR 5

typedef struct player_t
{
    int x, y;           // coordenadas del tetramino que cae (la posicion [0][0])
    int tipo, rotacion; // del tetramino que cae

    int new_x, new_y;           // coordenadas del proximo tetramino
    int new_tipo, new_rotacion; // del proximo tetramino

	int level, score, lines;
	char name[MAX_CHAR]; 
} player_t;

/***********************************
 *	        funciones
 ***********************************/
/**
 * @brief genera una pieza inicial y la siguiente aleatoriamente con su respectiva
 * posicion inicial
 * @param player_t struct con los datos del jugador
 */
void initGame(player_t *);

/**
 * @brief espera input del usuario con lo que sera el nombre del jugador
 * @param player_t struct con los datos del jugador
 */
void askForName(player_t *);

/**
 * @brief actualiza la pieza actual con la proxima y genera altetoriamente otra
 * proxima
 * @param player_t struct con los datos del jugador
 */
void createNewTetramino(player_t *);

void performMove(player_t *, char);

#endif // PLAYER_H
