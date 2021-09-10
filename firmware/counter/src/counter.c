#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"

void delay(word t)
{
    word delay = 0;

    while (delay < t)
    {
        asm("nop");
        delay++;
    }
}

int main()
{
    byte a = 0;

    while (true)
    {
        // Write the FPGA GPIO latch
        POKE(0x8C00, a);
        delay(1000);
        a++;
    }

    return 0;
}
