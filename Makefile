# Makefile for TPF programacion 1 (tetris)

OPTIONS = -g -Wall # debug and all warnings

# Source files
BACKEND_SRC = $(wildcard backend/*.c)
FRONTEND_SRC = $(wildcard frontend/*.c)
SRC = main.c ${BACKEND_SRC} ${FRONTEND_SRC}

# Headers
BACKEND_HEADERS = $(wildcard backend/*.h)
FRONTEND_HEADERS = $(wildcard frontend/*.h)
HEADERS = $(BACKEND_HEADERS) $(FRONTEND_HEADERS)

# Object files
BACKEND_OBJ = $(BACKEND_SRC:.c=.o)
FRONTEND_OBJ = $(FRONTEND_SRC:.c=.o)
OBJ = main.o ${BACKEND_OBJ} ${FRONTEND_OBJ}

# Compile for terminal
tetris: ${OBJ}
	gcc ${OPTIONS} ${OBJ} -o tetris

# Compile for Allegro
allegro: OPTIONS += -D ALLEGRO
allegro: ${OBJ}
	gcc ${OPTIONS} ${OBJ} -o tetris $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5)

# Compile for Raspberry Pi
raspi: OPTIONS += -D RASPI
raspi: ${OBJ} frontend/disdrv.o frontend/joydrv.o frontend/libAudioSDL2.o
	gcc ${OPTIONS} ${OBJ} frontend/disdrv.o frontend/joydrv.o frontend/libAudioSDL2.o -I/usr/local/include -L/usr/local/lib -lSDL2 -o tetris

# Build object files
$(BACKEND_OBJ): backend/%.o: backend/%.c ${BACKEND_HEADERS}
	gcc ${OPTIONS} -c $< -o $@

$(FRONTEND_OBJ): frontend/%.o: frontend/%.c ${FRONTEND_HEADERS}
	gcc ${OPTIONS} -c $< -o $@

main.o: main.c ${HEADERS}
	gcc ${OPTIONS} -c main.c

# Clean
clean:
	rm tetris ${OBJ}




