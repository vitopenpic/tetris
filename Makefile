# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = main.c tetramino.c board.c player.c control.c display.c score.c
HEADERS = tetramino.h board.h player.h control.h display.h score.h disdrv.h joydrv.h
OBJ = $(SRC:.c=.o)

tetris: ${OBJ}	# compile for terminal
	-gcc ${OPTIONS} ${OBJ} -o tetris

raspi: OPTIONS += -D RASPI # compile for raspberry pi
raspi: ${OBJ} disdrv.o joydrv.o
	-gcc ${OPTIONS} ${OBJ} disdrv.o joydrv.o -o tetris

${OBJ}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm tetris ${OBJ}


