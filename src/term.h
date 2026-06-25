#ifndef TERM_H
#define TERM_H

#include <termios.h>

void move_cursor(int x, int y);
void clear_screen(void);

void enter_raw_mode(void);
void exit_raw_mode(void);

#endif
