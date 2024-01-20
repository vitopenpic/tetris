/**************************************************************************
 * @file display.h
 * @authors
 * @brief archivo de cabecera que se encarga de la visualizacion del juego
 * tanto como para raspi, allegro y terminal
 **************************************************************************/

#include "player.h"

/**
 * @brief printea mScene a la terminal
 * */
void drawScene();

/**
 * @brief copia el contenido de mScene al display matricial de la
 * raspberry
 */
void drawInDisplay();

/**
 * @brief imprime el juego a la terminal
 */
void drawInTerminal(player_t *);

/**
 * @brief limpia la terminal
 * */
void clearScreen();

/**
 * @brief imprime la proxima pieza a la terminal
 * */
void printNextPiece(player_t *);
