/**************************************************************************
 * @file player.h
 * @authors
 * @brief archivo de cabecera que se encarga de la inicializacion del 
 * jugador, su puntaje, nivel y la creacion de las piezas
 **************************************************************************/

#ifndef GAME_H
#define GAME_H

/***********************************
 *	enums, defines, structs
 ***********************************/

typedef struct player_t
{
    int x, y;           // coordenadas del tetramino que cae (la posicion [0][0])
    int tipo, rotacion; // del tetramino que cae

    int new_x, new_y;           // coordenadas del proximo tetramino
    int new_tipo, new_rotacion; // del proximo tetramino

	int level, score, lines;
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
 * @brief actualiza la pieza actual con la proxima y genera altetoriamente otra
 * proxima
 * @param player_t struct con los datos del jugador
 */
void createNewTetramino(player_t *);

/**
 * @brief calcula cuanto puntaje se obtuvo en la ultima pasada
 * @param level, lines. el puntaje depende de estos
 * @return cantidad de puntos adquiridos
 */
int howMuchScore(int level,  int lines);

#endif // GAME_H
