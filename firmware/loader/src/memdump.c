#include <peekpoke.h>
#include "memdump.h"
#include "acia.h"
#include "print.h"

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