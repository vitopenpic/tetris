# Makefile para TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug y all warnings
SRC = main_test2.c tetramino.c board.c game.c raspi.c control.c
HEADERS = tetramino.h board.h game.h raspi.h control.h disdrv.h joydrv.h
OBJ = main_test2.o tetramino.o board.o game.o raspi.o control.o disdrv.o joydrv.o

main_test2: ${OBJ}
           -gcc ${OPTIONS} ${OBJ} -o main_test2

${OBJS}: ${SRC} ${HEADERS}
        -gcc ${OPTIONS} -c ${SRC}

clean:
        rm main_test2.o tetramino.o board.o game.o raspi.o control.o

