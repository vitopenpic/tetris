#include "backend/board.h"
#include "backend/player.h"
#include "backend/score.h"
#include "backend/menu.h"
#include "frontend/control.h"
#include "frontend/display.h"
#include "frontend/soundFX.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/**************************************************************************/

// Declaración de colores
ALLEGRO_COLOR colors[8];

int main(void)
{





	// inicializaciones ---------------------------------------------------
	srand((unsigned int)time(NULL)); // seed para rand()
	player_t player;
	char key;
#ifdef RASPI
	joy_init();
	disp_init();
	joyinfo_t joystick = {0, 0, J_NOPRESS};
	initSoundFX();
	playTitleScreenMusic();
	drawTitleScreen();
#elif ALLEGRO
	// Inicializar Allegro		
   	if (!al_init()) 
	{
		fprintf(stderr, "Failed to initialize Allegro!\n");
		return -1;
	}
    al_init_primitives_addon();
    al_init_image_addon();
	al_install_keyboard();




#ifdef ALLEGRO

  // Crear la cola de eventos						TECLADO
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());



// Función para detectar y procesar los eventos del teclado
void processKeyboardEvents(ALLEGRO_EVENT_QUEUE *event_queue, player_t *player) {
    ALLEGRO_EVENT event;
    while (al_get_next_event(event_queue, &event)) {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    performMove(player, LEFT);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    performMove(player, RIGHT);
                    break;
                case ALLEGRO_KEY_DOWN:
                    performMove(player, DOWN);
                    break;
                case ALLEGRO_KEY_UP:
                    performMove(player, ROTATE);
                    break;
                default:
                    break;
            }
        }
    }
}
#endif
	// Inicializar colores
	colors[0] = al_map_rgb(0, 0, 0);      // Negro
	colors[1] = al_map_rgb(255, 0, 0);    // Rojo
    colors[2] = al_map_rgb(0, 255, 0);    // Verde
    colors[3] = al_map_rgb(0, 0, 255);    // Azul
    colors[4] = al_map_rgb(255, 255, 0);  // Amarillo
    colors[5] = al_map_rgb(255, 165, 0);  // Naranja
    colors[6] = al_map_rgb(128, 0, 128);  // Morado
    colors[7] = al_map_rgb(0, 255, 255);  // Cyan


	

    // Crear la ventana  
    ALLEGRO_DISPLAY *display = al_create_display(800, 800);
    al_set_window_title(display, "Tetris");

	 

  // Inicializar otras variables y estructuras de datos
   
    initGame(&player);

  
	

    // Dibujar el tablero
	//dibutablero(display);
	drawTitle();


    // Refrescar la pantalla
    al_flip_display();
#else
	enableNonBlockingInput(); // desactiva ICANON mode
#endif
	do // espera a el nombre del jugador	
	{
		enterName(player.name);			//(espera input)
	} while (!confirmName(player.name)); 

	do // outer loop -------------------------------------------------------
	{
		initGame(&player);
		initMenu();

		// timer -----------------------------------------------------------
		double fallInterval = getSpeed(0); // en seg (rapidez inicial nivel 0)
		double startTime, currentTime;
		startTime = getTime();
#ifdef RASPI
		disp_clear();
		disp_update();
		double musicTimer = getTime();
		startMusic();
#endif

		do 	// inner loop --------------------------------------------------
		{
			// musica ------------------------------------------------------			
#ifdef RASPI
			musicTimer = refreshMusic(musicTimer);
#endif

			// input control -----------------------------------------------
#ifdef RASPI
			joystick = joy_read();
			key = whichKeyWasPressed(&joystick);
			performMove(&player, key); // puede cambiar el menuStatus a OPEN
#elif ALLEGRO 

  // Bucle principal
    while (!isGameOver()) {
        // Procesar eventos del teclado solo si el juego está en progreso
        if (menuStatus() == RESUME) {
            processKeyboardEvents(event_queue, &player);
        }


else
			if (kbhit()) // se pregunta si se presiono una tecla
			{
				key = getchar();
				performMove(&player, key); // puede cambiar el menuStatus a OPEN
			}
#endif
			// menu ---------------------------------------------------------
			while (menuStatus() == OPEN)
			{
#ifdef RASPI
				char prev_key = key;
				musicTimer = refreshMusic(musicTimer);
				joystick = joy_read();
				key = whichKeyWasPressed(&joystick);
				if (prev_key == key) continue;
#else
				key = getchar();
#endif
				navigateMenu(key);
				printMenu();
#ifdef RASPI
				if (key == ENTER)
					disp_clear();
#endif
			}

			// caida libre ---------------------------------------------------
			currentTime = getTime();
			double elapsedTime = currentTime - startTime;

			if (elapsedTime >= fallInterval)
			{
				startTime = currentTime;
				performMove(&player, DOWN);
			}

			// score/points & level ------------------------------------------
			int linesCombo = eraseLineIfFull();
			player.lines += linesCombo;
#ifdef RASPI
			int previousLevel = player.level;
#endif
			player.level = player.lines / 10;

#ifdef RASPI
			if (previousLevel != player.level)
				playLevelUpSound();
#endif
			player.score += howMuchScore(player.level, linesCombo);

			// rendering ------------------------------------------------------
			updateScene(&player);
#ifdef RASPI
			drawInRaspberry(&player);
#elif ALLEGRO
			drawInAllegro(&player);	
			}		
#else		
			drawInTerminal(&player);
#endif

			// updates speed ---------------------------------------------------
			fallInterval = getSpeed(player.level); // MAX_LEVEL es la rapidez max

			// delay
			usleep(20000); // = 0.02 seg. para que renderize suavemente

		} while (!isGameOver() && menuStatus() == RESUME); // end of inner loop-
		// retry menu ----------------------------------------------------------
		if (isGameOver())
		{
#ifdef RASPI
			playGameOverSound();	
			reverseClearDelay();		
#endif
			if(!confirmRetry())
				wantToExit();
		}
	} while (menuStatus() != EXIT); // end of outer loop ------------------------

	// lista mejores puntajes ---------------------------------------------------
	updateTopScore("leaderboard.dat", player.score, player.name);
	printTopScores();

	// frees y finalizacioens ---------------------------------------------------
#ifdef RASPI
	endSoundFX();
	disp_clear();
	disp_update();

#elif ALLEGRO
	// Finalizar y liberar recursos
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

#else
	restoreBlockingInput(); /* si al correr el codigo no se llega
	hasta aca, escribir 'stty sane' en la terminal para reestablecer
	la configuracion inicial luego de haber ejecutado main_test*/
#endif
	return 0;
}
