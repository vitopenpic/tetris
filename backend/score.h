#ifndef SCORE_H
#define SCORE_H
/**************************************************************************
 * @file score.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del puntaje
 **************************************************************************/

#include <stdio.h>
#include "player.h"

/***********************************
*	      defines, structs
 ***********************************/

#define MAX_SCORERS 10

struct Score
{
	char name[MAX_CHAR];
	int score, level;
};

/***********************************
 *	        funciones
 ***********************************/

/**
 * @brief calcula cuanto puntaje se obtuvo en la ultima pasada
 * @param level, lines. el puntaje depende de estos
 * @return cantidad de puntos adquiridos
 */
int howMuchScore(int level,  int lines);

/**
 * @brief lee y guarda la lista puntajes maximos a disco
 * @param filename nombre del archivo local donde se guarda
 * @param currentScore puntaje del jugador
 * @param currentLevel nivel del jugador
 * @param *currentName nombre del jugador
 */
void updateLeaderboard(const char *filename, int currentScore, int currentLevel,
					   char *currentName);

/**
 * @brief getter que deja acceder a la tabla de mejores puntajes 'leaderboard'
 * @param indice de la tabla
 * @return puntero de tipo Score
 */
struct Score *getLeaderboard(int i);

#endif // SCORE_H

