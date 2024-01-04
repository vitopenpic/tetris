/**************************************************************************
 * @file tetramino.h
 * @authors
 * @brief archivo de cabecera con los 'getters' para manejar los tetraminos
 **************************************************************************/

#ifndef TETRAMINO_H
#define TETRAMINO_H

#include <stdbool.h>

/***********************************
 *	        funciones
 ***********************************/
/**
 * @brief devuelve el contenido de la matriz mTetramino
 * @param tipo de tetramino (0-6)
 * @param rotacion del tetramino (0-3)
 * @param x @param y, posicion de la matriz mTetramino (posicion [0][0])
 * con respecto a mBoard
 */
bool getBlockType(int tipo, int rotacion, int x, int y);

/**
 * @brief devuelve la traslacion horizontal inicial para cada tipo de pieza 
 * @param tipo de tetramino (0-6)
 * @param rotacion del tetramino (0-3)
 */
int getInitPosX(int tipo, int rotacion);

/**
 * @brief devuelve la traslacion vertical inicial para cada tipo de pieza 
 * @param tipo de tetramino (0-6)
 * @param rotacion del tetramino (0-3)
 */
int getInitPosY(int tipo, int rotacion);

#endif // TETRAMINO_H
