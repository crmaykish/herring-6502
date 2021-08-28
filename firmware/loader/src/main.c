#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

void memdump(word address, byte bytes);

int main()
{
    char buffer[80];

    acia_init();

    // TODO: clear screen

    print("\r\nHerring 6502 Monitor v1.2\r\n");
    print("Colin Maykish - 2021\r\n");
    print("github.com/crmaykish/herring-6502\r\n\r\n");

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

void memdump(word address, byte bytes)
{
    word i = 0;
    byte b = 0;

    print_hex(address);
    print("  ");

    while (i < bytes)
    {
        b = PEEK(address + i);
        print_hex(b);
        acia_putc(' ');

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            print(" |");
            print_string_bin((char *)(address + i - 16), 16);

            print("|\r\n");
            print_hex(address + i);
            print("  ");
        }
        else if (i % 8 == 0)
        {
            acia_putc(' ');
        }
    }

    acia_putc('|');
    print_string_bin((char *)(address + i - 16), 16);
    acia_putc('|');
}