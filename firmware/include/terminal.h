#ifndef TERMINAL_H
#define TERMINAL_H

#include "herring.h"

#define DEFAULT_COLORS "0"
#define FG_COLOR_RED "31"
#define FG_COLOR_GREEN "32"
#define FG_COLOR_YELLOW "33"
#define FG_COLOR_BLUE "34"
#define FG_COLOR_MAGENTA "35"
#define FG_COLOR_CYAN "36"
#define FG_COLOR_BRIGHT_RED "91"
#define FG_COLOR_BRIGHT_GREEN "92"
#define FG_COLOR_BRIGHT_YELLOW "93"
#define FG_COLOR_BRIGHT_BLUE "94"
#define FG_COLOR_BRIGHT_MAGENTA "95"
#define FG_COLOR_BRIGHT_CYAN "96"

#define BG_COLOR_RED "41"
#define BG_COLOR_GREEN "42"
#define BG_COLOR_YELLOW "43"
#define BG_COLOR_BLUE "44"
#define BG_COLOR_MAGENTA "45"
#define BG_COLOR_CYAN "46"
#define BG_COLOR_BRIGHT_RED "101"
#define BG_COLOR_BRIGHT_GREEN "102"
#define BG_COLOR_BRIGHT_YELLOW "103"
#define BG_COLOR_BRIGHT_BLUE "104"
#define BG_COLOR_BRIGHT_MAGENTA "105"
#define BG_COLOR_BRIGHT_CYAN "106"

typedef enum
{
    CURSOR_UP = 'A',
    CURSOR_DOWN = 'B',
    CURSOR_RIGHT = 'C',
    CURSOR_LEFT = 'D'
} cursor_dir_e;

void term_set_color(char *color);

void cursor_move(cursor_dir_e dir, byte steps);
void cursor_set_col(byte col);
void cursor_set_pos(byte row, byte col);

void set_cursor_visible(bool visible);

void screen_clear();

#endif
