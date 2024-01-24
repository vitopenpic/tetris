/**************************************************************************
 * @file control.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del input
 **************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include "joydrv.h"
#include "disdrv.h"
#include "../backend/player.h"

/***********************************
 *	        defines
 ***********************************/
#define EXIT 'x'
#define LEFT 'a'
#define RIGHT 'd'
#define DOWN 's'
#define ROTATE 'w'

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

//void initSettings(void);

void performMove(player_t *, char);

#endif // CONTROL_H