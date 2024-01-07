#include "tetramino.h"
#include "board.h"
#include "game.h"
#include "joydrv.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
        int fallInterval = 1000; // 1 seg
        time_t startTime = time(NULL);

        srand((unsigned int)time(NULL)); // seed para rand()

        player_t player;
        initGame(&player);

        joyinfo_t joystick = {0, 0, J_NOPRESS};
        joy_init();

        do
        {
                // control
                joystick = joy_read();
                char key = whichKeyWasPressed(&joystick);
                usleep();

                switch (key)
                {
                case 'a':
                        if (isMovementLegal(player.tipo, player.rotacion,
                                            player.x - 1, player.y))
                        {
                                player.x--;
                        }
                        break;
                case 'd':
                        if (isMovementLegal(player.tipo, player.rotacion,
                                            player.x + 1, player.y))
                        {
                                player.x++;
                        }
                        break;
                case 's':
                        if (isMovementLegal(player.tipo, player.rotacion,
                                            player.x, player.y + 1))
                        {
                                player.y++;
                        }
                        else
                        {
                                storePieceInBoard(&player);
                                createNewTetramino(&player);
                        }
                        break;
                case 'w':
                        if (isMovementLegal(player.tipo, (player.rotacion + 1) % 4,
                                            player.x, player.y))
                        {
                                player.rotacion = (player.rotacion + 1) % 4;
                        }
                        break;
                }

                // caida libre
                time_t currentTime = time(NULL);
                double elapsedTime = difftime(currentTime, startTime) * 1000;

                if (elapsedTime >= fallInterval)
                {
                        startTime = currentTime;

                        if (isMovementLegal(player.tipo, player.rotacion,
                                            player.x, player.y + 1))
                        {
                                player.y++;
                        }
                        else
                        {
                                storePieceInBoard(&player);
                                createNewTetramino(&player);
                        }
                }

                // rendering
                updateScene(&player);
                drawInDisplay();

                eraseLineIfFull();

        } while (!isGameOver());

        puts("This is your end, cowboy...");
        return 0;
}
