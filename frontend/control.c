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

#ifdef RASPI
#define ALPHA_MAX 26
const static char aAlphabet[ALPHA_MAX] =
{A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};

static int parseThroughAlphabet(joyinfo_t *js, int position)
{
    bool block = true;
    static int indx;
    char c;
    while (block)
    {
        *js = joy_read();
        c = whichKeyWasPressed(js);
        if (c == DOWN)
        {
            block = false;
            indx = 0; //empieza por la 'A'
        }
        if (c == MENU) // MENU = UP
        {
            block = false;
            indx = ALPHA_MAX - 1; // empieza por la 'Z'
        }
    }
    while(c != ROTATE) // ROTATE = CONFIRM
    {
        *js = joy_read();
        c = whichKeyWasPressed(js);
        switch (c)
        {
        case DOWN:
            indx = (indx + 1) % ALPHA_MAX;
            break;
        case MENU:
            indx = (indx - 1) % ALPHA_MAX;
            break;
        }
        printIndexedLetter(indx, position); // from display.h
    }
    return indx;
}
#endif

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

void enterName(char name[])
{	
    puts("- Heigh ho! Enter thy four character name... -");
    char c;
 	for (int i = 0; i < MAX_CHAR - 1; i++) 
	{
        while (!isalpha(c = getchar())); 
        name[i] = c;
    }
	name[MAX_CHAR-1] = '\0';
	while (getchar() != '\n');
}

bool confirmName(char name[])
{
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
}

#ifdef RASPI
void *enterNameRasp(char *name, joyinfo_t *js)
{
    printNameScreen();
    for (int i = 0; i < MAX_CHAR - 1; i++) 
	{
        int indx = parseThroughAlphabet(js, i);
        name[i] = aAlphabet[indx];
    }
	name[MAX_CHAR-1] = '\0';
    return name;
}

/*bool confirmNameRasp(char *name, joyinfo_t *js)
{

}*/
#endif

