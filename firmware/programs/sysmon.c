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

        // Write the ASCII characters
        ACIA_WriteBuffer("  |");
        for (i; i < 16; i++)
        {
            data = peek(addr + index);

            if (data < 0x32 || data > 126)
            {
                ACIA_Write('.');
            }
            else
            {
                ACIA_Write(data);
            }

            index++;
        }
        
        ACIA_Write('|');

        ACIA_NewLine();

        i = 0;
    }
}

void ParseCommand(char *buffer)
{
    word addr;
    byte data;

    // byte addr_s[5];
    byte data_s[3];

    char *firstParam;
    char *secondParam;

    // Isolate the parameters
    firstParam = strchr(buffer, ' ') + 1; // Note: first param includes the second param too
    secondParam = strchr(firstParam, ' ') + 1;

    if (strncmp(buffer, "memdump", 7) == 0)
    {
        addr = IntegerValue(firstParam);
        memdump(addr);
    }
    else if (strncmp(buffer, "peek", 4) == 0)
    {
        addr = IntegerValue(firstParam);
        itoa(peek(addr), data_s, 16);
        ACIA_WriteBuffer(data_s);
    }
    else if (strncmp(buffer, "poke", 4) == 0)
    {
        addr = IntegerValue(firstParam);
        data = IntegerValue(secondParam);
        poke(addr, data);
    }
    else if (strncmp(buffer, "reset", 5) == 0)
    {
        // Soft reset the system monitor
        asm("jmp %w", PROGRAM_RAM);
    }
    else if (strncmp(buffer, "load", 4) == 0)
    {
        // Reset to the ROM loader program
        asm("jmp %w", ROM_PROGRAM);
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
