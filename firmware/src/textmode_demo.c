#include "herring.h"
#include <peekpoke.h>
#include <stdlib.h>
#include <stdio.h>
#include "serial.h"
#include "delay.h"
#include "vga.h"

void memdump(uint16_t address, uint8_t bytes)
{
    uint16_t i = 0;
    uint8_t b = 0;

    printf("%04X  ", address);

    while (i < bytes)
    {
        b = PEEK(address + i);
        printf("%02X ", b);

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            printf(" |");
            print_string_bin((char *)(address + i - 16), 16);

            printf("|\r\n%04X  ", address + i);
        }
        else if (i % 8 == 0)
        {
            printf(" ");
        }
    }

    printf("|");
    print_string_bin((char *)(address + i - 16), 16);
    printf("|");
}

int main()
{
    char a;

    textmode_clear();

    TEXTMODE_SET_COLOR(VGA_GREEN);

    printf("dump 1000\r\n");

    memdump(0x1000, 128);

    printf("\r\n\r\ndump 2000\r\n");

    memdump(0x2000, 128);

    // while (1)
    // {
    //     a = serial_getc();

    //     if (a == 0x1B)
    //     {
    //         POKE(0x8001, 0x00);
    //     }
    //     else if (a == '\r')
    //     {
    //         POKE(0x8000, 0x0A);
    //         POKE(0x8000, 0x0D);
    //     }
    //     else
    //     {
    //         POKE(0x8000, a);
    //     }
    // }

    return 0;
}
