#include <stdint.h>
#include <peekpoke.h>

#include "delay.h"
#include "herring.h"

void delay(uint16_t x)
{
    uint16_t i = 0;

    POKE(VIA0_ACR, 0x00);

    while (i < x)
    {
        POKE(VIA0_T2CL, 0xFF);
        POKE(VIA0_T2CH, 0xFF);

        while ((PEEK(VIA0_IFR) & 0b00100000) == 0)
        {
        }

        i++;
    }
}
