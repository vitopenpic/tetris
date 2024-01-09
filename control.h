#ifndef CONTROL_H
#define CONTROL_H


/* desactiva el ICANON mode para que se pueda
leer user input sin bloquear el codigo.
https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html*/
void enableNonBlockingInput();

/* reestablece el ICANON mode*/
void restoreBlockingInput();

/* lee el input del usuario*/
int kbhit(void);

double getTime(void);

#endif // CONTROL_H
