#ifndef GAME_H
#define GAME_H

/***********************************
 *	enums, defines, structs
 ***********************************/

typedef struct player_t
{
    int x, y;           // coordenadas del tetramino que cae (la posicion [0][0])
    int tipo, rotacion; // del tetramino que cae

    int new_x, new_y;           // coordenadas del proximo tetramino
    int new_tipo, new_rotacion; // del proximo tetramino
} player_t;

/***********************************
 *	        funciones
 ***********************************/

void initGame(player_t *);

void createNewTetramino(player_t *);

/**
*@brief: updates mScene with the dead pieces on the board and the falling one
*/
void updateScene(player_t*);

void drawScene();

void clearScene();

// void fallingTetramino();

int getRandInBetween(int a, int b);

#endif // GAME_H
