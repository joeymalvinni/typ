#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "ui.h"
#include "term.h"

void start(void) {}
void settings(void) {}
void quit(void) {}

int main(void) {
    MenuItem elements[] = {
        { "Start", start },
        { "Settings", settings },
        { "Quit", quit }
    };

    MenuLinks links = {
        .items = elements,
        .len = sizeof(elements)/sizeof(elements[0])
    };

    MenuConfig conf = {
        .title_text = "Test menu conf",
        .style = DOUBLE,
        .links = links,
        .x_padding = 10,
        .y_padding = 5,
        .inner_padding = 3,
        .selected = 1,
    };

    print_menu(&conf);
    return 0;

    enter_raw_mode();
    enter_alt_screen();

    char c;
    while (1) {
        if (read(STDIN_FILENO, &c, 1) == -1 && errno == EAGAIN) {
            printf("die");
            break;
        };
        clear_screen();

        print_menu(&conf);
        if (c == 'q') {
            break;
        }
    }

    exit_alt_screen();
    exit_raw_mode();
}
