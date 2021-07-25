#include "via.h"
#include "acia.h"
#include <peekpoke.h>

int main()
{
    unsigned int i = 0;
    char c = 32;

    ACIA_Init();

    ACIA_WriteBuffer("\r\nHerring 6502\r\n");

    for (;;)
    {
        VIA_SetPortA(i & 0xFF);
        VIA_SetPortB((i & 0xFF00) >> 8);
        ACIA_Write(c);

        i++;
        c++;

        if (c > 126)
        {
            c = 32;
        }
    }

    return 0;
}
