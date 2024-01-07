#include "raspi.h"

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

