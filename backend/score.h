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
void updateTopScore(const char *filename, int currentScore, int currentLevel,
					char *currentName);

/**
 * @brief imprime la lista de los mejores puntajes a la terminal
 */
void printTopScores(void);

#endif // SCORE_H
