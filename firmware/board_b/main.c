#include <peekpoke.h>
#include <stdio.h>
#include "acia.h"

#define SERIAL_PRINT_BUFFER_SIZE 64

// Print a formatted string to the serial console
#define aprintf(format, ...)                                         \
    snprintf(buffer, SERIAL_PRINT_BUFFER_SIZE, format, __VA_ARGS__); \
    ACIA_WriteBuffer(&SerialConsole, buffer)

int main()
{
    char buffer[SERIAL_PRINT_BUFFER_SIZE] = {0};
    
    unsigned char i = 0;
    char c = '0';

    ACIA_Init(&SerialConsole);

    for (;;)
    {
        aprintf("hello: %c\r\n", c);

        aprintf("more: %c | %s\r\n", c, "test");

        c++;

        if (c > 'z')
        {
            c = '0';
        }
    }

    return 0;
}
