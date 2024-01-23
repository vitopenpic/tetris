# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings
SRC = main.c tetramino.c board.c player.c control.c display.c score.c soundFX.c
HEADERS = tetramino.h board.h player.h control.h display.h score.h soundFX.h disdrv.h joydrv.h audio.h
OBJ = $(SRC:.c=.o)

# compile for terminal
tetris: ${OBJ}
	-gcc ${OPTIONS} ${OBJ} -o tetris

# compile for raspberry pi
raspi: OPTIONS += -D RASPI
raspi: ${OBJ} disdrv.o joydrv.o
	-gcc ${OPTIONS} ${OBJ} disdrv.o joydrv.o libAudioSDL2.o -I/usr/local/include -L/usr/local/lib -lSDL2 -o tetris

${OBJ}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm tetris ${OBJ}


