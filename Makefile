# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = tetramino.c board.c game.c control.c
HEADERS = tetramino.h board.h game.h control.h disdrv.h joydrv.h
OBJ = tetramino.o board.o game.o control.o disdrv.o joydrv.o

main_test: ${OBJ} main_test.o
    -gcc ${OPTIONS} ${OBJ} -o main_test

main_test2: ${OBJ} main_test2.o
    -gcc ${OPTIONS} ${OBJ} -o main_test2

${OBJ}: ${SRC} main_test.c main_test2.c ${HEADERS}
    -gcc ${OPTIONS} -c ${SRC}

clean:
    rm main_test.o tetramino.o board.o game.o control.o main_test2.o


