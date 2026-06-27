#ifndef SCREEN_H
#define SCREEN_H

#include "term.h"

typedef struct {
    char character;
    char *style[]; // TODO: implement colors
} Cell; // single ascii cell for our screen

#endif
