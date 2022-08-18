#include "herring.h"
#include <peekpoke.h>
#include <stdlib.h>
#include <stdio.h>
#include "serial.h"
#include "delay.h"

void vga_print(char *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        POKE(0x8000, s[i]);
        ++i;
    }
}

int main()
{
    char a;

    // clear screen
    POKE(0x8001, 0x00);
    delay(20);

    printf("HELLO FROM 6502\r\n");
    printf("SOME NUMBERS: %d %d", 1234, 0x4BEE);

    while (1)
    {
        a = serial_getc();

        if (a == 0x1B)
        {
            POKE(0x8000, 0x7F);
        }
        else if (a == '\r')
        {
            POKE(0x8000, 0x0A);
            POKE(0x8000, 0x0D);
        }
        else
        {
            POKE(0x8000, a);
        }
    }

    return 0;
}
