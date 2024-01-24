#ifndef DISPLAY_H
#define DISPLAY_H
/**************************************************************************
 * @file display.h
 * @authors
 * @brief archivo de cabecera que se encarga de la visualizacion del juego
 * tanto como para raspi, allegro y terminal
 **************************************************************************/

#include "player.h"

/**
 * @brief imprime el juego al display matricial de la
 * raspberry
 */
void drawInRaspberry(player_t *);

/**
 * @brief imprime el juego a la terminal
 */
void drawInTerminal(player_t *);

#endif // DISPLAY_H
