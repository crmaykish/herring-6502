#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "via.h"
#include "utils.h"

// Note: global variables mess with the reset vector and everything breaks...

void memdump(word addr)
{
    byte data;
    char data_s[3];
    char addr_s[6];
    byte index = 0;
    byte i = 0;
    byte j = 0;

    // TODO: write ascii after the hexdump

    for (j; j < 8; j++)
    {
        itoa(addr + index, addr_s, 16);

        ACIA_WriteBuffer(addr_s);
        ACIA_WriteBuffer("  ");

        for (i; i < 16; i++)
        {
            data = peek(addr + index);
            itoa(data, data_s, 16);

            if (data < 0x10)
            {
                ACIA_Write('0');
            }

            ACIA_WriteBuffer(data_s);

            if (i == 7)
            {
                ACIA_Write(' ');
            }

            ACIA_Write(' ');

            index++;
        }
        i = 0;
        ACIA_NewLine();
    }
}

void ParseCommand(char *buffer)
{
    if (strncmp(buffer, "memdump", 7) == 0)
    {
        word addr;

        // Convert address string to int
        addr = atoi(buffer + 8);

        memdump(addr);
    }
    else if (strncmp(buffer, "peek", 4) == 0)
    {
        byte addr_s[6];
        byte data_s[3];
        byte data;
        word addr;

        addr = IntegerValue(buffer + 5);

        itoa(addr, addr_s, 16);

        ACIA_WriteBuffer(addr_s);
        ACIA_WriteBuffer(": ");

        data = peek(addr);
        itoa(data, data_s, 16);
        ACIA_WriteBuffer(data_s);
    }
    else if (strncmp(buffer, "poke", 4) == 0)
    {
    }
    else if (strncmp(buffer, "cls", 3) == 0)
    {
        byte i = 0;

        for (i; i < 40; i++)
        {
            ACIA_NewLine();
        }
    }
    else if (strncmp(buffer, "free", 4) == 0)
    {
        size_t free_ram = _heapmaxavail();

        utoa(free_ram, buffer, 10);

        ACIA_WriteBuffer("Free RAM: ");
        ACIA_WriteBuffer(buffer);
        ACIA_WriteBuffer(" bytes");
    }
}

int main()
{
    char buffer[40];

    ACIA_Init();

    ACIA_WriteBuffer("Herring 6502 ><(((°>");
    ACIA_NewLine();

    while (true)
    {
        ACIA_WriteBuffer("> ");
        ACIA_ReadLine(buffer, 39, true);
        ACIA_NewLine();

        ParseCommand(buffer);

        ACIA_NewLine();

        memset(buffer, 0, 40);
    }

    return 0;
}
