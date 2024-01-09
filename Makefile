# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = main_test.c tetramino.c board.c game.c control.c
HEADERS = tetramino.h board.h game.h control.h disdrv.h joydrv.h
OBJ = main_test.o tetramino.o board.o game.o control.o

main_test: ${OBJ}
	-gcc ${OPTIONS} ${OBJ} -o main_test

main_test_raspi: ${OBJ} disdrv.o joydrv.o
	-gcc -D RASPI ${OPTIONS} ${OBJ} disdrv.o joydrv.o -o main_test

${OBJ}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm main_test.o tetramino.o board.o game.o control.o


