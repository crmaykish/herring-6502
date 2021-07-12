#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "via.h"

// TODO: Compiler is doing something weird with ASCII characters - lower case goes to upper, upper are not recognized

void ParseCommand(char *buffer)
{
    // Get the instruction portion of the command (everything up to the first delimiter)

    if (strncmp(buffer, "peek", 4) == 0)
    {
        word addr;
        byte data;
        char addr_s[6];
        char data_s[3];
        byte i = 0;
        byte j = 0;
        byte index = 0;

        // Convert address string to int
        strncpy(addr_s, buffer + 5, 6);
        addr = atoi(addr_s);

        for (j; j < 8; j++)
        {
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
    else if (strncmp(buffer, "poke", 4) == 0)
    {
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
