#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

// TODO: add rounded corners -- only for thin pipe i think
const char *VERT_PIPE[4] = {" ", "│", "┃", "║"};
const char *HORIZ_PIPE[4] = {" ", "─", "━", "═"};
const char *TOP_LEFT_CORNER[4] = {" ", "┌", "┏", "╔"};
const char *TOP_RIGHT_CORNER[4] = {" ", "┐", "┓", "╗"};
const char *BOT_LEFT_CORNER[4] = {" ", "└", "┗", "╚"};
const char *BOT_RIGHT_CORNER[4] = {" ", "┘", "┛", "╝"};

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
char *build_border_line(LineType type, size_t line_len, MenuStyle style) {
    const char *left = NULL;
    const char *right = NULL;
    const char *horiz = HORIZ_PIPE[style];

    if (type == TOP) {
        left = TOP_LEFT_CORNER[style];
        right = TOP_RIGHT_CORNER[style];
    } else if (type == BOTTOM) {
        left = BOT_LEFT_CORNER[style];
        right = BOT_RIGHT_CORNER[style];
    } else {
        printf("Undefined behavior");
        return "";
    }

    size_t total =
        strlen(left) +
        (line_len * strlen(horiz)) +
        strlen(right) +
        1; // null terminator

    char *buf= malloc(total);
    char *p = buf;

    strcpy(p, left);
    p += strlen(left);

    for (size_t i = 0; i < line_len; ++i) {
        strcpy(p, horiz);
        p += strlen(horiz);
    }

    strcpy(p, right);
    p += strlen(right);

    strcpy(p, "\0");

    return buf;
}

char *build_title_line(const char* title_text, size_t full_inner_length, MenuStyle style) {
    int max_len = strlen(VERT_PIPE[style])*2 + full_inner_length + 1;
    char *buf = malloc(max_len);
    char *p = buf;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);

    snprintf(p, max_len, "%*s", (int)(full_inner_length-strlen(title_text))/2, " ");
    p += (full_inner_length-strlen(title_text))/2;
    snprintf(p, max_len, "%s", title_text);
    p += strlen(title_text);
    snprintf(p, max_len, "%*s", (int)(full_inner_length-strlen(title_text))/2, " ");
    p += (full_inner_length-strlen(title_text))/2;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);

    strcpy(p, "\0");

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
    char *p = buf;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);

    snprintf(p, max_inner_string_length, "%*s", left_padding, " ");
    p += left_padding;

    snprintf(p, max_inner_string_length, "%s%s", (curr == *selected) ? SELECTED_LIST : DESELECTED_LIST, label);
    p += strlen(SELECTED_LIST) + strlen(label);

    snprintf(p, max_inner_string_length, "%*s", right_padding, " ");
    p += right_padding;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);
    strcpy(p, "\0");

    return buf;
}

char *build_empty_line(MenuStyle style, size_t full_inner_length) {
    char *buf = malloc(full_inner_length + strlen(VERT_PIPE[style])*2 + 1);
    char *p = buf;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);

    snprintf(p, full_inner_length + 1, "%*s", (int)full_inner_length, " ");
    p += full_inner_length;

    strcpy(p, VERT_PIPE[style]);
    p += strlen(VERT_PIPE[style]);

    return buf;
}

void print_empty_line(MenuStyle style, size_t full_inner_length) {
    printf("%s", VERT_PIPE[style]);

    printf("%*s", (int)full_inner_length, ""); 

    printf("%s", VERT_PIPE[style]);
    printf("\n");
}

void print_menu(MenuConfig *config) {
    int line_len = strlen(config->title_text) + (config->x_padding*2);
    size_t max_link_length = get_max_links_label_length(config->links);
    size_t target_inner_length = (max_link_length > (size_t)line_len) ? (size_t)max_link_length : line_len; // max inner length of one line without padding
    size_t full_inner_length = target_inner_length + config->x_padding*2;

    // top line
    char *top_line = build_border_line(TOP, full_inner_length, config->style);
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
            // print_empty_line(config->style, full_inner_length);
            char *empty = build_empty_line(config->style, full_inner_length);
            printf("%s\n", empty);
        }
    }

    char *bot_line= build_border_line(BOTTOM, full_inner_length, config->style);
    printf("%s\n", bot_line);
}
