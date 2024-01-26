/**************************************************************************
 * @file player.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del menu
 **************************************************************************/

#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#define ENTER 'x'
#define DOWN 's'
#define UP 'w'

typedef enum menu_status_t
{
	RESUME, // = 0
	RESTART,
	EXIT,
	OPEN
} menu_status_t;

menu_status_t menuStatus();

void initMenu();

void setMenuStatusOpen();

void navigateMenu(char key);

#endif // MENU_H
