#ifndef TERM_H
#define TERM_H

#include <termios.h>

typedef enum {
    BlinkingBlock = 1,
    SteadyBlock = 2,
    BlinkingUnderline = 3,
    SteadyUnderline = 4,
    BlinkingBar = 5,
    SteadyBar = 6
} CursorStyle;

void move_cursor(int x, int y);
void clear_screen(void);
void hide_cursor(void);
void show_cursor(void);
void change_cursor_style(CursorStyle style);

void enter_alt_screen(void);
void exit_alt_screen(void);
void enter_raw_mode(void);
void exit_raw_mode(void);

#endif
