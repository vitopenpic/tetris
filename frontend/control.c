#include "control.h"
#include "display.h"
#include "soundFX.h"
#include "../backend/player.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

/**************************************************************************
 *                          FUNCIONES AUX RASPBERRY
 **************************************************************************/

#ifdef RASPI
#define ALPHA_MAX 26
const static char aAlphabet[ALPHA_MAX] =
{'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
'O','P','Q','R','S','T','U','V','W','X','Y','Z'};

static int modulo(int a, int b)
{
    return (a % b + b) % b;
}
static int parseThroughAlphabet(int position)
{
    static joyinfo_t js = {0, 0, J_NOPRESS};
    bool block = true;
    int indx;
    char c;
    while (block)
    {
        js = joy_read();
        c = whichKeyWasPressed(&js);
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
        char prev_c = c;
        js = joy_read();
        c = whichKeyWasPressed(&js);
        if (prev_c == c) continue;
        switch (c)
        {
        case DOWN:
            indx = modulo(indx + 1, ALPHA_MAX);
            break;
        case MENU:
            indx = modulo(indx - 1, ALPHA_MAX);
            break;
        }
        printIndexedLetter(indx, position); // from display.h
    }
    playLockSound();
    return indx;
}

static bool yesOrNoRasp()
{
    bool state = false;
    joyinfo_t js = {0, 0, J_NOPRESS};
    char c;
    do
    {
        js = joy_read();
        c = whichKeyWasPressed(&js);
        switch (c)
        {
        case LEFT:
            state = true;
            break;
        case RIGHT: // MENU = UP
            state = false;
            break;
        }
        printYesOrNo(state);    // from display.h
    } while(c != ROTATE);  // ROTATE = CONFIRM
    return state;
}
#endif

/**************************************************************************
 *                          FUNCIONES PARA TERMINAL
 **************************************************************************/

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

/**************************************************************************
 *                          FUNCIONES PARA RASPBERRY
 **************************************************************************/
#ifdef RASPI
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

int indexOfLboard(int i)
{
    int prev_i = i;
    static joyinfo_t js = {0, 0, J_NOPRESS};
    static char key = 0;
    do
    {
        char prev_key = key;
        js = joy_read();
        key = whichKeyWasPressed(&js);
        if (prev_key == key) continue;
        switch (key)
        {
        case MENU: // MENU = UP
            i = modulo(i - 1, 10);
            break;
        case DOWN:
            i = modulo(i + 1, 10);
            break;
        case ROTATE:
            i = -1;
            break;
        }
    } while(prev_i == i);
    return i;
}
#endif
/**************************************************************************
 *                          FUNCIONES EN COMUN
 **************************************************************************/

double getTime()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1.0e9; // en segundos
}

void enterName(char name[])
{	
#ifdef RASPI
    printNameScreen();
    for (int i = 0; i < MAX_CHAR - 1; i++) 
    {
        int indx = parseThroughAlphabet(i);
        name[i] = aAlphabet[indx];
    }
    name[MAX_CHAR-1] = '\0';
#else
    puts("- Heigh ho! Enter thy four character name... -");
    char c;
 	for (int i = 0; i < MAX_CHAR - 1; i++) 
	{
        while (!isalpha(c = getchar())); 
        name[i] = c;
    }
	name[MAX_CHAR-1] = '\0';
	while (getchar() != '\n');
#endif
}

#define NAME_HEIGHT 1
bool confirmName(char name[])
{
#ifdef RASPI
    disp_clear();
    disp_update();
    printString2Rasp(name, NAME_HEIGHT);
    return yesOrNoRasp();
#else
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
#endif
}

bool confirmRetry()
{
#ifdef RASPI
    disp_clear();
    disp_update();
    printString2Rasp("RTRY", NAME_HEIGHT);
    return yesOrNoRasp();
#else
    char input;
    bool isValidInput = false;

    while (!isValidInput) 
    {
        puts("\nWant to retry? [Y/N]");
        input = getchar();
        printf("%c\n", input);
        while (getchar() != '\n');
        switch (input) 
        {
            case 'Y':
            case 'y':
                return true;
            case 'N':
            case 'n':
                return false;
            default:
                puts("Invalid input. Please enter 'Y' or 'N'");
        }
    }
    return isValidInput;
#endif
}

