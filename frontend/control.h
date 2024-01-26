/**************************************************************************
 * @file control.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del input
 **************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include "joydrv.h"
#include "disdrv.h"

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
int kbhit(void);

double getTime(void);

char whichKeyWasPressed(joyinfo_t *);

#endif // CONTROL_H
