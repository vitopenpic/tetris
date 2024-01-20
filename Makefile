# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = main_test.c tetramino.c board.c player.c control.c display.c score.c
HEADERS = tetramino.h board.h player.h control.h disdrv.h joydrv.h display.h score.h
OBJ = $(SRC:.c=.o)

tetris: ${OBJ}
	-gcc ${OPTIONS} ${OBJ} -o tetris

raspi: OPTIONS += -D RASPI
raspi: ${OBJ} disdrv.o joydrv.o
	-gcc ${OPTIONS} ${OBJ} disdrv.o joydrv.o -o main_test

${OBJ}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm tetris ${OBJ}


