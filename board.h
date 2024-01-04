/**************************************************************************
 * @file board.h                                                    
 * @authors 
 * @brief                                                              
 **************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include "tetramino.h"
#include "game.h"

/***********************************
*	const, enums, defines
***********************************/

#define BOARD_WIDTH 8 // ancho en bloques
#define BOARD_HEIGHT 16 // alto en bloques
#define BLOCKS_PER_PIECE 5 // dimension de la matriz de los tetraminos

enum {FREE, OCCUPIED}; // estados posibles del tablero para cada bloque

/***********************************
*	        funciones
***********************************/


bool isMovementLegal(player_t *);

/**
* @brief guarda un tetramino en la matriz de juego (board)
* @param x posicion horizontal del elemento [0][0] de la matriz de tetramino en 
  bloques,
  @param y idem pero vertical
  @param tipo de tetramino (0-7),
* @param rotacion de este (0-3),
*/
void storePieceInBoard(player_t *);

// void eraseLineIfFull();

// bool isGameOver();

/**
 * @brief pone todos los bloques de la mScene en FREE=0 */
void clearScene();

void updateScene(player_t *);

void drawScene();



#endif //BOARD_H
