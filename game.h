/**************************************************************************
 * @file game.h
 * @authors
 * @brief archivo de cabecera que se encarga de la inicializacion del juego
 * y la creacion de las pizas
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

	int level, score, lines_deleted;
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

#endif // GAME_H
