/**************************************************************************
 * @file board.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del tablero y las
 * mecanicas del juego
 **************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include "tetramino.h"
#include "game.h"

/***********************************
 *	     enums, defines
 ***********************************/
#ifdef RASPI

#define BOARD_WIDTH 8      // ancho en bloques
#define BOARD_HEIGHT 16    // alto en bloques

#else

#define BOARD_WIDTH 10     
#define BOARD_HEIGHT 20 

#endif 

#define BLOCKS_PER_PIECE 5 // dimension de la matriz de los tetraminos
#define MAX_LEVEL 29
 
enum
{
  FREE,
  OCCUPIED
}; // estados posibles del tablero para cada bloque


/***********************************
 *	        funciones
 ***********************************/
/**
 * @brief indica si en el estado actual de mScene hay
 * una colision (mov ilegal) o no (legal)
 * @param player_t struct con los datos del jugador
 * @return un booleano que responde a la funcion
 */
bool isMovementLegal(int tipo, int rotacion, int x, int y);

/**
 * @brief guarda un tetramino en la matriz de juego (board)
 * @param player_t struct con los datos del jugador
 */
void storePieceInBoard(player_t *);

/**
 * @brief pone todos los bloques de la mScene en FREE=0
 * */
void clearScene();

/**
 * @brief actualiza el contenido de mScene con los tetraminos muertos y
 * con el tetramino en juego
 * @param player_t struct con los datos del jugador
 */
void updateScene(player_t *);

/**
 * @brief printea mScene a la terminal
 * */
void drawScene();

/**
 * @brief copia el contenido de mScene a el display matricial de la
 * raspberry
 */
void drawInDisplay();

/**
 * @brief si una linea esta completa mueve todas una hacia abajo
 * @return cantidad de lineas eliminadas simultaneamente
 * */
int eraseLineIfFull();

/**
 * @brief devuelve un booleano respondiendo si el juego termino o no
 * */
bool isGameOver();

/**
 * @brief limpia la terminal
 * */
void clearScreen();

/**
 * @brief imprime la proxima pieza a la terminal
 * */
void printNextPiece(player_t *);

/**
 * @brief devuelve la rapidez de la look up table aSpeed dependiendo
 * del nivel actual
 * */
double getSpeed(int level);

#endif // BOARD_H
