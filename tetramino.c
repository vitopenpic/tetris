/**************************************************************************
 * @file tetramino.h
 * @authors
 * @brief
 **************************************************************************/

#include "tetramino.h"
#include <stdbool.h>
#include <stdio.h>

/*definicion tetraminos. hay de 7 tipos (O, I, L, J, Z, S, T), cada uno
con 4 rotaciones y cada uno es representado como una matriz de 5x5
bloques. el pivot de rotacion es siempre el bloque [2][2] salvo para O*/
static const bool mTetramino[7][4][5][5] = /* la matriz se ordena por
[tipo][rotacion][bloques verticales][bloques horizontales]*/
    {
        // O
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}}},

        // I
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 1, 1},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {1, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}}},
        // L
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // J
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}}},
        // Z
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // S
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // T
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}}};

/*definicion de la matriz de traslaciones iniciales para cada
tetramino*/
static const int mPosInit[7][4][2] = //[tipo][rotacion][x_0, y_0]
    {
        // O
        {
            {-2, -2},
            {-2, -2},
            {-2, -2},
            {-2, -2}},
        // I
        {
            {-2, -2},
            {-2, -1},
            {-2, -2},
            {-2, 0}},
        // L
        {
            {-2, -1},
            {-2, -2},
            {-2, -1},
            {-2, -1}},
        // J
        {
            {-2, -1},
            {-2, -1},
            {-2, -1},
            {-2, -2}},
        // Z
        {
            {-2, -1},
            {-2, -2},
            {-2, -1},
            {-2, -1}},
        // S
        {
            {-2, -1},
            {-2, -2},
            {-2, -1},
            {-2, -1}},
        // T
        {
            {-2, -1},
            {-2, -2},
            {-2, -1},
            {-2, -1}},
};

bool getBlockType(int tipo, int rotacion, int x, int y)
{
    if (tipo > 6 || rotacion > 3 || x > 4 || y > 4)
    {
        puts("Error in getBlockType");
        return 0;
    }

    return mTetramino[tipo][rotacion][y][x];
}

int getInitPosX(int tipo, int rotacion)
{
    if (tipo > 6 || rotacion > 3)
    {
        puts("Error in getInitPosX");
        return 0;
    }

    return mPosInit[tipo][rotacion][0];
}

int getInitPosY(int tipo, int rotacion)
{
    if (tipo > 6 || rotacion > 3)
    {
        puts("Error in getInitPosY");
        return 0;
    }

    return mPosInit[tipo][rotacion][1];
}
