#include <stdlib.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

typedef enum
{
    UP = 'A',
    DOWN = 'B',
    RIGHT = 'C',
    LEFT = 'D'
} cursor_dir_e;

void font_red()
{
    print("\033[31m");
}

void font_green()
{
    print("\033[32m");
}

void font_reset()
{
    print("\033[0m");
}

void move_cursor(cursor_dir_e dir, byte steps)
{
    char command[10];
    itoa(steps, command, 10);
    print("\033[");
    print(command);
    putc((char)dir);
}

void set_column(byte col)
{
    print("\033[");
    print_dec(col);
    putc('G');
}

void set_position(byte row, byte col)
{
    print("\033[");
    print_dec(row);
    putc(';');
    print_dec(col);
    putc('H');
}

void screen_clear()
{
    print("\033[2J\033[H");
}

int main()
{
    byte i = 0;
    byte j = 0;

    screen_clear();

    font_green();
    set_position(5, 5);
    print("Conway's Game of Life");
    font_reset();

    set_column(40);

    font_red();
    putc('X');

    font_reset();

    for (i=0; i < 24; i++)
    {
        set_position(i, i);
        putc('#');
    }

    return 0;
}
