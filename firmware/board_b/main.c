#include <stdio.h>
#include <peekpoke.h>
#include "acia.h"

#define PRINT(format, ...)                     \
    snprintf(buffer, 63, format, __VA_ARGS__); \
    ACIA_WriteBuffer(&SerialConsole, buffer)

int main()
{
    char buffer[64] = {0};
    unsigned char i = 0;
    char c = '0';

    POKE(0xC002, 0xFF);
    POKE(0xC003, 0xFF);

    ACIA_Init(&SerialConsole);

    for (;;)
    {
        PRINT("hello: %c\r\n", c);

        PRINT("more: %c | %s\r\n", c, "test");

        c++;

        if (c > 'z')
        {
            c = '0';
        }
    }

    return 0;
}
