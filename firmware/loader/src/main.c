#include <stdbool.h>
#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"
#include "memdump.h"

int main()
{
    char buffer[80];

    acia_init();

    screen_clear();

    font_green();
    print("Herring 6502 Monitor v1.2\r\n");

    font_red();
    print("Colin Maykish - 2021\r\n");

    font_cyan();
    print("github.com/crmaykish/herring-6502\r\n\r\n");

    font_reset();

    memdump(0xE000, 128);

    print("\r\n");

    while (true)
    {
        print("> ");
        readline(buffer, true);

        print("\r\ngot: ");
        print(buffer);
        print("\r\n");
    }

    return 0;
}
