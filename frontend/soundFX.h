#ifndef SOUNDFX_H
#define SOUNDFX_H
/**************************************************************************
 * @file soundFX.h
 * @authors
 * @brief archivo de cabecera que se encarga del manejo de audio
 **************************************************************************/
#include "audio.h"

/**
 * @brief inicializa el audio para la raspberry. llamar solo una vez 
 * */
void initSoundFX();

/**
 * @brief inicia la lista de temas del juego 
 * */
void startMusic();

/**
 * @brief funcion que cambia la musica si un tema de la lista de reproduccion 
 * termino o no y refresca el timer de duracion de los temas
 * @param timer que lleva la duracion de la temas
 * @param mismo timer, solo que lo resetea si termino el tema
 * */
double refreshMusic(double timer);

/**
 * @brief libera memoria dinamica utilizada por initSoundFX
 * */
void endSoundFX();

/**
 * @brief inicia el tema de inicio de juego
 * */
void playTitleScreenMusic();

/**
 * @brief activa el sonido de 'click1'
 * */
void playLockSound();

/**
 * @brief activa el sonido de 'levelUP'
 * */
void playLevelUpSound();

/**
 * @brief activa el sonido de 'lineCleared'
 * */
void playLineClearSound();

/**
 * @brief activa el sonido de 'gameOver'
 * */
void playGameOverSound();

/**
 * @brief pausa la musica
 * */
void stopMusic();

#endif // SOUNDFX_H

