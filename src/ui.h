#ifndef UI_H
#define UI_H

typedef enum {
    NONE = 0,
    THIN = 1,
    BOLD = 2,
    DOUBLE = 3
} MenuStyle;

typedef enum {
    TOP,
    BOTTOM,
    MIDDLE
} LineType;

typedef enum {
    ROUNDED = 4,
    STRAIGHT // default
} CornerType;

typedef struct {
    char *label;
    void (*func)(void);
} MenuItem;

typedef struct {
    MenuItem *items;
    int len;
} MenuLinks;

typedef struct {
    const char *title_text;
    MenuStyle style;
    MenuLinks links;
    CornerType corners;
    int x_padding;
    int y_padding;
    int inner_padding;
    int selected;
} MenuConfig;

void print_menu(MenuConfig *config);

#endif // UI_H
