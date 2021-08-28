#ifndef TERMINAL_H
#define TERMINAL_H

#include "herring.h"

typedef enum
{
    CURSOR_UP = 'A',
    CURSOR_DOWN = 'B',
    CURSOR_RIGHT = 'C',
    CURSOR_LEFT = 'D'
} cursor_dir_e;

void font_red();
void font_green();
void font_blue();
void font_cyan();
void font_reset();

void cursor_move(cursor_dir_e dir, byte steps);
void cursor_set_col(byte col);
void cursor_set_pos(byte row, byte col);

void screen_clear();

#endif
