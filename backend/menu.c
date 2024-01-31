#include "menu.h"
#include "../frontend/soundFX.h"
#include <stdio.h>
#include <stdlib.h>

static menu_status_t status; // estado de la accion a realizar por el menu
static int status_index;	 /* indice que sirve para navegar
 por el menu, una vez apretado enter se guarda su valor en status. uso int en vez de
 menu_status_t por tema de aritmetica modular en navigateMenu() */

static int modulo(int a, int b)
{
    return (a % b + b) % b;
}

menu_status_t menuStatus()
{
	return status;
}

int menuIndex()
{
	return status_index;
}

void initMenu()
{
	status = RESUME;
	status_index = RESUME;
}

void setMenuStatusOpen()
{
	status = OPEN;
}

void navigateMenu(char key)
{
	switch (key)
	{ // OPEN no es una opcion a elegir, por eso el % 3
	case DOWN:
		status_index = modulo(status_index + 1, 3);
#ifdef RASPI
		playLockSound();
#endif
		break;
	case UP:
		status_index = modulo(status_index - 1, 3);
#ifdef RASPI
		playLockSound();
#endif
		break;
	case ENTER:
		status = abs(status_index); // status deja de valer OPEN
		break;
	}
}

void wantToRetry(char key)
{
	if (key == 'n')
		status = EXIT;
}
