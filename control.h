#ifndef CONTROL_H
#define CONTROL_H

/* desactiva el ICANON mode para que se pueda
leer user input sin bloquear el codigo.
https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html*/
void enableNonBlockingInput();

/* reestablece el ICANON mode*/
void restoreBlockingInput();

/* returns 1 si se presiono una tecla*/
int kbhit(void);

char whichKeyWasPressed(joyinfo_t *);

#endif // CONTROL_H
