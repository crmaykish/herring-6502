#include <stdlib.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

void memdump(word address, byte bytes);

int main()
{
    char buffer[40];
    word address = 0;
    byte bytes = 0;

    while (true)
    {
        print("> ");
        readline(buffer, true);
        print("\r\n");

        address = (word)strtol(buffer, 0, 16);
        bytes = 128;

        memdump(address, bytes);

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
        putc(' ');

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
            putc(' ');
        }
    }

    putc('|');
    print_string_bin((char *)(address + i - 16), 16);
    putc('|');
}