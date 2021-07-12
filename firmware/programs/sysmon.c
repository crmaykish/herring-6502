#include "herring.h"
#include "acia.h"
#include "via.h"

// TODO: Compiler is doing something weird with ASCII characters - lower case goes to upper, upper are not recognized

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
        ACIA_WriteBuffer(buffer);
        ACIA_NewLine();
    }

    return 0;
}
