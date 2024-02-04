#ifndef DISPLAY_H
#define DISPLAY_H
/**************************************************************************
 * @file display.h
 * @authors
 * @brief archivo de cabecera que se encarga de la visualizacion del juego
 * tanto como para raspi, allegro y terminal
 **************************************************************************/

#include "../backend/player.h"
#include <stdbool.h>
#ifdef ALLEGRO
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#endif
/**************************************************************************
 *                          FUNCIONES PARA TERMINAL
 **************************************************************************/
/**
 * @brief imprime el juego en terminal
 * @param player_t un puntero de tipo player_t
 */
void drawInTerminal(player_t *);

/**************************************************************************
 *                          FUNCIONES PARA RASPBERRY
 **************************************************************************/
/**
 * @brief imprime el juego al display matricial de la
 * raspberry
 * @param player_t un puntero de tipo player_t
 */
void drawInRaspberry(player_t *);

/**
 * @brief borra el display con delay de arriba abajo
 */
void reverseClearDelay();

/**
 * @brief imprime la title-screen para la raspberry
 */
void drawTitleScreen();

/**
 * @brief imprime en el display "I AM" y donde el jugador vera su nombre
 */
void printNameScreen();

/**
 * @brief imprime la letra a seleccionar para el nombre del jugador
 * @param letter recibe un indice del 0-26 significando las letras A-Z
 * @param x_position posicion horizontal donde se imprime la letra segun
 * coordenadas del display (0-16)
 */
void printIndexedLetter(int letter, int x_position);

/**
 * @brief imprime un string de maximo 4 letras al display
 * @param string a imprimir
 * @param height posicion vertical donde se imprime la palabra segun
 * coordenadas del display (0-16)
 */
void printString2Rasp(char *string, int height);

/**
 * @brief imprime en display YES or NO dependiendo del state
 * @param state true = yes; false = no
 */
void printYesOrNo(bool state);

/**************************************************************************
 *                       FUNCIONES PARA ALLEGRO
 **************************************************************************/
// faltan comentarios
void dibuTablero();
void drawInAllegro(player_t *);
void initAllegro();
void processKeyboardEvents(player_t *);
void drawTitle();
void destroyAllegro();

/**************************************************************************
 *                FUNCIONES CON COMPILACION CONDICIONAL
 **************************************************************************/
/**
 * @brief imprime el contenido del menu
 */
void printMenu();

/**
 * @brief imprime la tabla de mejores puntajes
 */
void printLeaderboard();

#endif // DISPLAY_H
