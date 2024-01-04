# Makefile para TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug y all warnings
SRC = tetramino.c board.c game.c main_test.c
HEADERS = tetramino.h board.h game.h
OBJ = tetramino.o board.o game.o main_test.o

main_test: ${OBJ}
	   -gcc ${OPTIONS} ${OBJ} -o main_test

${OBJS}: ${SRC} ${HEADERS}
	-gcc ${OPTIONS} -c ${SRC}

clean:
	rm *.o
