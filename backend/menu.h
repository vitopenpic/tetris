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

/*posibles estados del menu*/
typedef enum menu_status_t
{
	RESUME, // = 0
	RESTART,
	EXIT,
	OPEN
} menu_status_t;

/**
 * @brief devuelve el estado actual del menu
 */
menu_status_t menuStatus();

/**
 * @brief indice que significa la opcion que se muestra en el menu
 * @return devuelve 0-2, o sea RESUME/RESTART/EXIT
 */
int menuIndex();

/**
 * @brief inicializa las variables necesarias para el menu. llamar solo una vez
 */
void initMenu();

/**
 * @brief pone el estado del menu como OPEN
 */
void setMenuStatusOpen();

/**
 * @brief segun la tecla, deja aumnetar el indice del menu
 * @param key es la tecla que fue presionada
 */
void navigateMenu(char key);

/**
 * @brief el estado del menu pasa a EXIT
 */
void wantToExit();

#endif // MENU_H
