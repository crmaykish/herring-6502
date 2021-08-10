#include <stdlib.h>
#include "herring.h"
#include "acia.h"

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

int main()
{
    font_green();
    print("Conway's Game of Life");
    font_reset();

    move_cursor(LEFT, 4);

    font_red();
    putc('X');

    return 0;
}
