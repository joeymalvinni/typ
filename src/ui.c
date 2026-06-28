#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

const char *VERT_PIPE[4] = {" ", "│", "┃", "║"};
const char *HORIZ_PIPE[4] = {" ", "─", "━", "═"};
const char *TOP_LEFT_CORNER[5]  = {" ", "┌", "┏", "╔", "╭"};
const char *TOP_RIGHT_CORNER[5] = {" ", "┐", "┓", "╗", "╮"};
const char *BOT_LEFT_CORNER[5]  = {" ", "└", "┗", "╚", "╰"};
const char *BOT_RIGHT_CORNER[5] = {" ", "┘", "┛", "╝", "╯"};

const char *DESELECTED_LIST= "[ ] ";
const char *SELECTED_LIST = "[x] ";

size_t get_max_links_label_length(MenuLinks links) {
    size_t target_line_width = 0;

    for (int i = 0; i < links.len; ++i) {
        size_t line_length = strlen(links.items[i].label) + strlen(DESELECTED_LIST); // may be an array eventually
        if (line_length > target_line_width ) {
            target_line_width = line_length;
        }
    }

    target_line_width += (target_line_width % 2 == 0) ? 0 : 1; // make even

    return target_line_width;
}

// build either the top or the bottom
char *build_border_line(LineType type, size_t line_len, MenuStyle style, CornerType corners) {
    const char *left = NULL;
    const char *right = NULL;
    int corner_style = (corners == ROUNDED) ? ROUNDED : style;
    if (type == TOP) {
        left = TOP_LEFT_CORNER[corner_style];
        right = TOP_RIGHT_CORNER[corner_style];
    } else if (type == BOTTOM) {
        left = BOT_LEFT_CORNER[corner_style];
        right = BOT_RIGHT_CORNER[corner_style];
    } else {
        fprintf(stderr, "Undefined behavior");
        return NULL;
    }

    const char *horiz = HORIZ_PIPE[style];
    size_t l_len = strlen(left);
    size_t r_len = strlen(right);
    size_t h_len = strlen(horiz);

    size_t total = l_len + line_len*h_len + r_len + 1;

    char *buf= malloc(total);
    if (!buf) return NULL;
    char *p = buf;

    p = stpcpy(p, left);

    for (size_t i = 0; i < line_len; ++i) {
        memcpy(p, horiz, strlen(horiz));
        p += strlen(horiz);
    }

    p = stpcpy(p, right);

    return buf;
}

char *build_title_line(const char* title_text, size_t full_inner_length, MenuStyle style) {
    int max_len = strlen(VERT_PIPE[style])*2 + full_inner_length + 1;
    char *buf = malloc(max_len);
    if (!buf) return NULL;
    char *p = buf;


    p = stpcpy(p, VERT_PIPE[style]);

    memset(p, ' ', (int)(full_inner_length-strlen(title_text))/2);
    p += (full_inner_length-strlen(title_text))/2;
    p = stpcpy(p, title_text);

    memset(p, ' ', (int)(full_inner_length-strlen(title_text))/2);
    p += (full_inner_length-strlen(title_text))/2;

    p = stpcpy(p, VERT_PIPE[style]);

    return buf;
}

char *build_link_line(MenuLinks *links, MenuStyle style, size_t full_inner_length, size_t max_link_length, int y_padding, int i, int *selected) {
    int curr = i-(2+y_padding);
    char *label = links->items[curr].label;
    int left_padding = (full_inner_length - max_link_length) / 2;
    int right_padding = full_inner_length - left_padding - (int)strlen(label) - (int)strlen(DESELECTED_LIST);

    size_t max_inner_string_length = 
                strlen(VERT_PIPE[style])*2
                + left_padding
                + strlen(DESELECTED_LIST)
                + strlen(label)
                + right_padding
                + 1;
    char *buf = malloc(max_inner_string_length);
    if (!buf) return NULL;
    char *p = buf;

    p = stpcpy(p, VERT_PIPE[style]);

    memset(p, ' ', left_padding);
    p += left_padding;

    p = stpcpy(p, (curr == *selected) ? SELECTED_LIST : DESELECTED_LIST);
    p = stpcpy(p, label);

    memset(p, ' ', right_padding);
    p += right_padding;

    p = stpcpy(p, VERT_PIPE[style]);

    return buf;
}

char *build_empty_line(MenuStyle style, size_t full_inner_length) {
    char *buf = malloc(full_inner_length + strlen(VERT_PIPE[style])*2 + 1);
    if (!buf) return NULL;
    char *p = buf;

    p = stpcpy(p, VERT_PIPE[style]);

    memset(p, ' ', full_inner_length);
    p += full_inner_length;

    p = stpcpy(p, VERT_PIPE[style]);

    return buf;
}

void print_menu(MenuConfig *config) {
    int line_len = strlen(config->title_text) + (config->x_padding*2);
    size_t max_link_length = get_max_links_label_length(config->links);
    size_t target_inner_length = (max_link_length > (size_t)line_len) ? (size_t)max_link_length : line_len; // max inner length of one line without padding
    size_t full_inner_length = target_inner_length + config->x_padding*2;

    // top line
    char *top_line = build_border_line(TOP, full_inner_length, config->style, config->corners);
    printf("%s\n", top_line);
 
    // middle body
    for (int i = 0; i < config->links.len + (config->y_padding*2) + 2; i++) {
        if (i == config->y_padding) {
            char* title = build_title_line(config->title_text, full_inner_length, config->style);
            printf("%s\n", title);
        } else if (i > config->y_padding + 1 && i < config->links.len + 2 + config->y_padding) { // +1 = 1 space for padding between menu title
            char *link = build_link_line(&config->links, config->style, full_inner_length, max_link_length, config->y_padding, i, &config->selected);
            printf("%s\n", link);
        } else {
            char *empty = build_empty_line(config->style, full_inner_length);
            printf("%s\n", empty);
        }
    }

    char *bot_line= build_border_line(BOTTOM, full_inner_length, config->style, config->corners);
    printf("%s\n", bot_line);
}
