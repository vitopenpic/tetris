# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = main_test.c tetramino.c board.c player.c control.c display.c
HEADERS = tetramino.h board.h player.h control.h disdrv.h joydrv.h display.h
OBJ = $(SRC:.c=.o)

main_test: ${OBJ}
	-gcc ${OPTIONS} ${OBJ} -o main_test

main_test_raspi: OPTIONS += -D RASPI
main_test_raspi: ${OBJ} disdrv.o joydrv.o
	-gcc ${OPTIONS} ${OBJ} disdrv.o joydrv.o -o main_test

${OBJ}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm main_test ${OBJ}


