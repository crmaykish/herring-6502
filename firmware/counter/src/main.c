#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"

void print_hex(word w)
{
    char s[7] = {0};
    utoa(w, s, 16);

    if (w < 0x10)
    {
        putc('0');
    }

    print(s);
}

void memdump(word addr, byte rows);

int main()
{
    bool running = true;
    byte input[40] = {0};

    print("ASM65\r\n");

    while (running)
    {
        print("$ ");
        readline(input, true);
        print("\r\n");

        if (strncmp(input, "memdump", 7) == 0)
        {
            memdump(0x1000, 8);
        }
        else if (strncmp(input, "exit", 4) == 0)
        {
            running = false;
        }

        print("\r\n");
    }

    print("Exiting.\r\n");

    return 0;
}

void memdump(word addr, byte rows)
{
    byte i = 0;
    byte j = 0;
    byte m = 0;

    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < 16; i++)
        {
            m = PEEK(addr + (16 * j) + i);
            print_hex(m);
            putc(' ');
            
            if (i == 7)
            {
                putc(' ');
            }
        }

        print("\r\n");
    }
}
