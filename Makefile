# Makefile para TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug y all warnings
SRC = main_test.c tetramino.c board.c game.c control.c
HEADERS = tetramino.h board.h game.h control.h
OBJ = main_test.o tetramino.o board.o game.o  control.o

main_test: ${OBJ}
	   -gcc ${OPTIONS} ${OBJ} -o main_test

${OBJS}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm *.o
