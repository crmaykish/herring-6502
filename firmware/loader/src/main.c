#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "loader.h"

void memdump(unsigned int address);

int main()
{
    char in = 0;

    acia_init();

    print("Herring 6502\r\n");

    while (true)
    {
        print("> ");
        in = getc();

        putc(in);

        print("\r\n");

        switch (in)
        {
        case 'h':
            // Print the help message
            print("Commands: (h)elp, (l)oad, (r)un, (m)emdump");
            break;
        case 'l':
            print("LOAD");
            load_from_serial();
            break;
        case 'r':
            print("RUN");
            run_loaded_program();
            break;
        case 'm':
            memdump(0x8000);
            break;
        default:
            print("Bad command");
            break;
        }

        print("\r\n");
    }

    return 0;
}

void memdump(unsigned int address)
{
    byte i = 0;
    byte b = 0;

    for (i; i < 0xF0; ++i)
    {
        b = PEEK(address + i);

        if (b >= 32 && b < 127)
        {
            putc(b);
        }
        else
        {
            putc('.');
        }

        if ((i & 0xF) == 0xF) // If we're at a multiple of 16 (0-indexed)
        {
            print("\r\n");
        }
        else
        {
            putc(' ');
        }
    }
}
