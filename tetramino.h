/**************************************************************************
 * @file tetramino.h                                                    
 * @authors 
 * @brief                                                              
 **************************************************************************/

#ifndef TETRAMINO_H
#define TETRAMINO_H

#include <stdbool.h>

/***********************************
*	enums, defines
***********************************/

/***********************************
*	        funciones
***********************************/

bool getBlockType(int tipo, int rotacion, int x, int y);

int getInitPosX(int tipo, int rotacion);

int getInitPosY(int tipo, int rotacion);

#endif //TETRAMINO_H
