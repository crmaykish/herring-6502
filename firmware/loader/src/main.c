#include "acia.h"
#include <peekpoke.h>

int main()
{
    unsigned int i = 0;

    ACIA_Init();

    ACIA_WriteBuffer("\r\nHerring 6502\r\n");

    for (;;)
    {
        POKE(0xC000, (i & 0xFF00) >> 8);
        POKE(0xC001, i & 0xFF);
        ACIA_WriteByte(i);

        i++;
    }

    return 0;
}
