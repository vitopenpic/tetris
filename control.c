#include "control.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

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

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

#define THRESHOLD 50

char whichKeyWasPressed(joyinfo_t *coord)
{
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
                if (coord->sw == J_PRESS)
                {
                        return 'w';
                }
}


    return 0;
}
