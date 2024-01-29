#ifndef DISPLAY_H
#define DISPLAY_H
/**************************************************************************
 * @file display.h
 * @authors
 * @brief archivo de cabecera que se encarga de la visualizacion del juego
 * tanto como para raspi, allegro y terminal
 **************************************************************************/

#include "../backend/player.h"

/**
 * @brief imprime el juego al display matricial de la
 * raspberry
 */
void drawInRaspberry(player_t *);

/**
 * @brief imprime la title screen para la raspberry
 */
void drawTitleScreen();

/**
 * @brief imprime el contenido del menu a la terminal
 */
void printMenu();

/**
 * @brief imprime el juego a la terminal
 */
void drawInTerminal(player_t *);

/**
 * @brief imprime la pantalla donde el jugador vera su nombre
 */
void printNameScreen();

/**
 * @brief imprime la letra a seleccionar
 */
void printIndexedLetter(int indx, int position);

#endif // DISPLAY_H
