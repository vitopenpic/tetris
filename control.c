#include "control.h"
#include "board.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>

void enableNonBlockingInput()
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreBlockingInput()
{
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


double getTime() 
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1.0e9;
}

#define THRESHOLD 75

char whichKeyWasPressed(joyinfo_t *coord)
{
    static bool sw_released; // para no mantener apretado el sw
    if (coord->sw == J_NOPRESS)
    {
        sw_released = true;
    }
    if (coord->x > THRESHOLD)
    {
        return 'd';
    }
    if (coord->x < -THRESHOLD)
    {
        return 'a';
    }
    if (coord->y < -THRESHOLD)
    {
        return 's';
    }
    if (coord->sw == J_PRESS && sw_released)
    {
        sw_released = false;
        return 'w';
    }
	else return -1;
}

void initSettings()
{
	#ifdef RASPI
	joyinfo_t joystick = {0, 0, J_NOPRESS};
	joy_init();
	return;
	
	#else
	enableNonBlockingInput(); // desactiva ICANON mode
	return;

	#endif
}

void performMove(player_t *player, char key)
{
	switch (key)
	{
	case 'a':
		if (isMovementLegal(player->tipo, player->rotacion,
							player->x - 1, player->y))
		{
			player->x--;
		}
		break;
	case 'd':
		if (isMovementLegal(player->tipo, player->rotacion,
							player->x + 1, player->y))
		{
			player->x++;
		}
		break;
	case 's':
		if (isMovementLegal(player->tipo, player->rotacion,
							player->x, player->y + 1))
		{
			player->y++;
		}
		else
		{
			storePieceInBoard(player);
			createNewTetramino(player);
		}
		break;
	case 'w':
		if (isMovementLegal(player->tipo, (player->rotacion + 1) % 4,
							player->x, player->y))
		{
			player->rotacion = (player->rotacion + 1) % 4;
		}
		break;
	}
}


