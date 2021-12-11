#include <stdlib.h>
#include <stdbool.h>
#include "terminal.h"
#include "acia.h"
#include "print.h"

#define CURSOR_HIDE "\033[?25l"
#define CURSOR_SHOW "\033[?25h"

void term_set_color(char *color)
{
    print("\033[");
    print(color);
    print("m");
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

void set_cursor_visible(bool visible)
{
    if (visible)
    {
        print(CURSOR_SHOW);
    }
    else
    {
        print(CURSOR_HIDE);
    }
}

void screen_clear()
{
    print("\033[2J\033[H");
}
