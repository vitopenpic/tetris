/**************************************************************************
 * @file score.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del puntaje
 **************************************************************************/

#include <stdio.h>

/***********************************
 *	          structs
 ***********************************/

struct Score
{
	char name[4];
	int score;
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

void writeScores(const char *filename, struct Score *scores, size_t numScores);

void readScores(const char *filename, struct Score *scores, size_t numScores);
