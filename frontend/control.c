#include "control.h"
#include "../backend/player.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

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

int kbhit()
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
    return now.tv_sec + now.tv_nsec / 1.0e9; // en segundos
}

#define THRESHOLD 75

char whichKeyWasPressed(joyinfo_t *coord)
{
	usleep(70000); // un poco de delay pq si no responde mal
	static bool sw_released; // para no mantener apretado el sw
    if (coord->sw == J_NOPRESS)
    {
        sw_released = true;
    }
    if (coord->x > THRESHOLD)
    {
        return RIGHT;
    }
    if (coord->x < -THRESHOLD)
    {
        return LEFT;
    }
	if (coord->y > THRESHOLD)
    {
        return MENU;
    }
    if (coord->y < -THRESHOLD)
    {
        return DOWN;
    }
    if (coord->sw == J_PRESS && sw_released)
    {
        sw_released = false;
        return ROTATE;
    }
    else
        return -1;
}

void *enterName(char *name)
{	
//#ifdef RASPI

//#else
	char c;
 	for (int i = 0; i < MAX_CHAR - 1; i++) 
	{
        while (!isalpha(c = getchar())); 
        name[i] = c;
    }
	name[MAX_CHAR-1] = '\0';
	while (getchar() != '\n');
//#endif
	return name;
}

bool confirmName(char name[])
{
//#ifdef RASPI
//#else
	printf("- Outlandish! Thou really goes by the name of %s? - [Y/N]\n", name);
	char c = getchar();
	while(getchar() != '\n');
	if (c == 'Y' || c == 'y')
	{
		puts("- I hast never heard such name... cool -");
		return true;
	}
	else if (c == 'N' || c == 'n') 	
	{
		puts("- I beg yerr pardon. Tell me again, how thou art called? -");
		return false;		
	}
	else
	{
		puts("- Stop mumbling gibberish! Doth thou not speak english? Tell me how thou art called... -");
		return false;
	}
//#endif
}

