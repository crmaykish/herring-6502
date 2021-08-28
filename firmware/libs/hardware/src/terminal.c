#include <stdlib.h>
#include "terminal.h"
#include "acia.h"
#include "print.h"

void font_red()
{
    print("\033[31m");
}

void font_green()
{
    print("\033[32m");
}

void font_blue()
{
    print("\033[34m");
}

void font_cyan()
{
    print("\033[36m");
}

void font_reset()
{
    print("\033[0m");
}

void cursor_move(cursor_dir_e dir, byte steps)
{
    char command[10];
    itoa(steps, command, 10);
    print("\033[");
    print(command);
    acia_putc((char)dir);
}

void cursor_set_col(byte col)
{
    print("\033[");
    print_dec(col);
    acia_putc('G');
}

void cursor_set_pos(byte row, byte col)
{
    print("\033[");
    print_dec(row);
    acia_putc(';');
    print_dec(col);
    acia_putc('H');
}

void screen_clear()
{
    print("\033[2J\033[H");
}