#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "via.h"

// TODO: Compiler is doing something weird with ASCII characters - lower case goes to upper, upper are not recognized

void ParseCommand(char *buffer)
{
    if (strncmp(buffer, "memdump", 7) == 0)
    {
        word addr;
        byte data;
        char addr_s[6];
        char data_s[3];
        byte i = 0;
        byte j = 0;
        byte index = 0;

        // Convert address string to int
        strncpy(addr_s, buffer + 8, 6);
        addr = atoi(addr_s);

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

    else if (strncmp(buffer, "cls", 3) == 0)
    {
        byte i = 0;

        for (i; i < 80; i++)
        {
            ACIA_NewLine();
        }
    }
    else if (strncmp(buffer, "free", 4) == 0)
    {
        size_t free_ram = _heapmaxavail();

        utoa(free_ram, buffer, 10);

        ACIA_WriteBuffer("free ram: ");
        ACIA_WriteBuffer(buffer);
        ACIA_WriteBuffer(" bytes");
    }
}

int main()
{
    char buffer[40];

    ACIA_Init();

    ACIA_WriteBuffer("herring 6502 ><>");
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
