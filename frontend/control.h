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
 * @brief recibe y escribe input del usuario para el nombre del jugador
 * compatible con raspberry
 * @param name puntero a un string
 * @param js informacion sobre el input del joystick
 */
void enterNameRasp(char *name, joyinfo_t *js);

/**
 * @brief revalida el nombre recibido
 * @param string con el nombre recibido
 * @return afirmativo o negativo
 */
// bool confirmNameRasp(char *name, joyinfo_t *js);

#endif // CONTROL_H
