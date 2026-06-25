#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    enter_raw_mode();

    print_menu(&conf);
}
