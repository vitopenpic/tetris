/**************************************************************************
 * @file control.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del input
 **************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include "joydrv.h"
#include "disdrv.h"
#include <stdbool.h>

/***********************************
 *	        funciones
 ***********************************/
/* desactiva el ICANON mode para que se pueda leer user input sin bloquear
el programa con el getchar().
https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html*/
void enableNonBlockingInput();

/* reestablece el ICANON mode*/
void restoreBlockingInput();

/* devuelve 1 si se presiono una tecla*/
int kbhit();

double getTime();

char whichKeyWasPressed(joyinfo_t *);

/**
 * @brief recibe y escribe input del usuario para el nombre del jugador
 * @param name, puntero a un string
 */
void enterName(char *name);

/**
 * @brief revalida el nombre recibido
 * @param string con el nombre recibido
 * @return afirmativo o negativo
 */
bool confirmName(char *name);

/**
 * @brief recibe input del jugador sobre si desea reintentar
 * @return afirmativo o negativo
 */
bool confirmRetry();

/**
 * @brief devuelve el numero del top10 que se esta mirando en la raspberry
 * @return numero 1-10 o -1 si se el jugador cerro el juego
 */
int indexOfLboard(int i);
#endif // CONTROL_H
