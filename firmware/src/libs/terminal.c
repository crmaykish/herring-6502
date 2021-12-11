#include <stdlib.h>
#include "terminal.h"
#include "acia.h"
#include "print.h"

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

void screen_clear()
{
    print("\033[2J\033[H");
}