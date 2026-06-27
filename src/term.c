#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "term.h"

static struct termios original_term;

void move_cursor(int x, int y) {
    printf("\x1b[%d;%dH", y, x);
}

void clear_screen(void) {
    printf("\x1b[2J\x1b[H");
}

void hide_cursor(void) {
    printf("\x1b[?25l");
}

void show_cursor(void) {
    printf("\x1b[?25h");
}

void enter_alt_screen(void) {
    // TODO: not use ascii escapes
    printf("\x1b[?1049h");
}

void exit_alt_screen(void) {
    printf("\x1b[?1049l");
}

void change_cursor_style(CursorStyle style) {
    printf("\033[%d q", style);
}

void enter_raw_mode(void) {
    int fd = STDIN_FILENO;
    tcgetattr(fd, &original_term); // copy the inital terminal state into original_termios_p

    struct termios raw = original_term;
    /* input modes - clear indicated ones giving: no break, no CR to NL, 
    no parity check, no strip char, no start/stop output (sic) control */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* output modes - clear giving: no post processing such as NL to CR+NL */
    raw.c_oflag &= ~(OPOST);
    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);
    /* local modes - clear giving: echoing off, canonical off (no erase with 
    backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // timeouts
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    atexit(exit_raw_mode); // ??
}

void exit_raw_mode(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_term) == -1)
            printf("failed to exit raw mode"); // TODO: proper error handling
}
