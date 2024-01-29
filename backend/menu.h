/**************************************************************************
 * @file player.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo del menu
 **************************************************************************/

#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#ifdef RASPI
#define ENTER 'w'
#define DOWN 's'
#define UP 'x'
#else
#define ENTER 'x'
#define DOWN 's'
#define UP 'w'
#endif

typedef enum menu_status_t
{
	RESUME, // = 0
	RESTART,
	EXIT,
	OPEN
} menu_status_t;

menu_status_t menuStatus();

int menuIndex();

void initMenu();

void setMenuStatusOpen();

void navigateMenu(char key);

void wantToRetry(char key);

#endif // MENU_H
