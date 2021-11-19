#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

int main()
{
    char in;

    while (true)
    {
        in = acia_getc();

        if (in == 0x0A || in == 0x0D)
        {
            // new line
        }
        else if (in == 32)
        {
            // space
            POKE(0x9000, 0);
        }
        else if (in >= 65 && in < 128)
        {
            // visible ASCII
            POKE(0x9000, in - 64);
        }
    }

    return 0;
}