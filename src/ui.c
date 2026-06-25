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

void print_top_line(size_t line_len, MenuStyle *style) {
    printf("%s", TOP_LEFT_CORNER[*style]);
    for (size_t i = 0; i < line_len; i++) {
        printf("%s", HORIZ_PIPE[*style]);
    }
    printf("%s", TOP_RIGHT_CORNER[*style]);
    printf("\n");
}


void print_bot_line(size_t line_len, MenuStyle *style) {
    printf("%s", BOT_LEFT_CORNER[*style]);
    for (size_t i = 0; i < line_len; i++) {
        printf("%s", HORIZ_PIPE[*style]);
    }
    printf("%s", BOT_RIGHT_CORNER[*style]);
    printf("\n");
}

void print_title_line(const char *title_text, size_t full_inner_length, MenuStyle *style) {
    printf("%s", VERT_PIPE[*style]);

    printf("%*s", (int)(full_inner_length-strlen(title_text))/2, "");
    printf("%s", title_text);
    printf("%*s", (int)(full_inner_length-strlen(title_text))/2, "");

    printf("%s", VERT_PIPE[*style]);
    printf("\n");
}

void print_link_line(MenuLinks *links, MenuStyle *style, size_t full_inner_length, size_t max_link_length, int y_padding, int i, int *selected) {
    printf("%s", VERT_PIPE[*style]);
    int curr = i-(2+y_padding);
    char *label = links->items[curr].label;
    int left_padding = (full_inner_length - max_link_length) / 2;
    int right_padding = full_inner_length - left_padding - (int)strlen(label) - (int)strlen(DESELECTED_LIST);

    printf("%*s", left_padding, "");
    printf("%s%s", (curr == *selected) ? SELECTED_LIST : DESELECTED_LIST, label);
    printf("%*s", right_padding, "");

    printf("%s", VERT_PIPE[*style]);
    printf("\n");
}

void print_empty_line(MenuStyle *style, size_t full_inner_length) {
    printf("%s", VERT_PIPE[*style]);

    printf("%*s", (int)full_inner_length, ""); 

    printf("%s", VERT_PIPE[*style]);
    printf("\n");
}

void print_menu(MenuConfig *config) {
    int line_len = strlen(config->title_text) + (config->x_padding*2);
    size_t max_link_length = get_max_links_label_length(config->links);
    size_t target_inner_length = (max_link_length > (size_t)line_len) ? (size_t)max_link_length : line_len; // max inner length of one line without padding
    size_t full_inner_length = target_inner_length + config->x_padding*2;

    // top line
    print_top_line(full_inner_length, &config->style);

    // middle body
    for (int i = 0; i < config->links.len + (config->y_padding*2) + 2; i++) {
        if (i == config->y_padding) {
            print_title_line(config->title_text, full_inner_length, &config->style);
        } else if (i > config->y_padding + 1 && i < config->links.len + 2 + config->y_padding) { // +1 = 1 space for padding between menu title
            print_link_line(&config->links, &config->style, full_inner_length, max_link_length, config->y_padding, i, &config->selected);
        } else {
            // printf("printing empty line: i=%d, y_padding=%d, links.len=%d, calc=%d", i, y_padding, links.len, links.len+2+y_padding);
            print_empty_line(&config->style, full_inner_length);
        }
    }

    print_bot_line(full_inner_length, &config->style);
}
